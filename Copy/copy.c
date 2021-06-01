#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void help();									//fucntion to print usage info
void copy_read_write(int fd_from, int fd_to);					//function to copy using read() and write()
void copy_mmap(int fd_from, int fd_to);						//function to copy using mmap()
const char** assign_files(int argc, char** argv, bool mflag);			//additional function to assign files from command line parameters

int main(int argc, char *argv[])
{
	if(argc == 1)
	{
		help();
		exit(0);
	}
	
	bool mflag = false;							//flag indicating if mmap() will be used
	int arg;
	
	while((arg = getopt(argc, argv, ":mh")) != -1)				//getopt() parses the command-line arguments
	{									//getopy() returns -1 when there are no command-line arguments left	
		switch(arg)
		{
			case 'm':						//if m is detected
				mflag = true;
				break;
				
			case 'h':						//if h is detected
				help();
				exit(0);
			
			case '?':						//if any other symbol is detected
				perror("Wrong syntax! Showing usage:\n\n");
				help();
				exit(1);
		}
	}
	
	if((mflag == false && argc != 3) || (mflag == true && argc != 4))	//checking if the number of command-line arguments is correct
	{
		perror("Wrong number of parameters! Showing usage:\n\n");
		help();
		exit(1); 
	}
	
	const char** files;
	files = assign_files(argc, argv, mflag);				//calling helper function to assign source and destination files
	
	int fd_from = open(files[1], O_RDONLY);					//open source file in read only mode
	if(fd_from == -1)
	{
		perror("Failed to open source file\n");
		exit(1);
	}
	
	int fd_to = open(files[2], O_RDWR | O_CREAT, 0666);			//open destination file in read write mode if the file exist or creating new file (mode 0666 gives proper permission)
	if(fd_to == -1)
	{
		perror("Failed to open destination file\n");
		exit(1);
	}
	
	if(mflag)
		copy_mmap(fd_from, fd_to);					//call copy_mmap() if -m was provided in command-line arguments
	else
		copy_read_write(fd_from, fd_to);				//call copy_read_write() if -m was not provided in command-line parameters
	
	
	if(close(fd_from) == 0)							//close source file
		printf("Source file closed with a success!\n");
	if(close(fd_to) == 0)							//close destination file
		printf("Destination file closed with a success!\n");
		
	exit(0);
}

const char** assign_files(int argc, char** argv, bool mflag)
{
	static const char* files_array[2];					//array to store source file as argument 1 and dest file as argument 2

	if(mflag)								//if "-m" mode then searching througn command-line arguments for files
	{
		bool source_file_assigned = false;				//flag to indicate if the first file was detected
		for(int i = 1; i < argc; i++)					//go through all command-line arguments
		{
			if(strcmp(argv[i], "-m") != 0)				//if argument is not '-m' then treating it as a file
			{
				if(!source_file_assigned)			
				{	
					files_array[1] = argv[i];		//store source file in array
					source_file_assigned = true;		//source file was assigned to flag set to true
				}
				else
					files_array[2] = argv[i];		//store destination file in array
			}
		}
	}
	else									//mode without "-m"
	{
		files_array[1] = argv[1];					//store source file in array
		files_array[2] = argv[2];					//store destination file in array
	}
	return files_array;
}

void copy_read_write(int fd_from, int fd_to)					
{
	struct stat stat_from;							//definition of a struct which will store info about a file
	if(fstat(fd_from, &stat_from) == -1)					//fstat() now in stat_from there is stored information about fd_from file
	{
		perror("Error in loading source file mode\n");
		exit(1);
	}
	char buffer[stat_from.st_size];						//buffer has size of the source file in bytes
	
	int read_file, write_file;
	
	while((read_file = read(fd_from, buffer, stat_from.st_size)) > 0)	//reading data from the source file
	{
		write_file = write(fd_to, buffer, stat_from.st_size);		//writing data to the destination file
		if(write_file <= 0)
		{
			perror("Error in writting to the file\n");
			exit(1);
		}
	}
}

void copy_mmap(int fd_from, int fd_to)
{
	struct stat stat_from;							//definition of a struct which will store info about a file
	if(fstat(fd_from, &stat_from) == -1)					//fstat() now in stat_from there is stored information about fd_from file
	{
		perror("Error in loading source file mode\n");
		exit(1);
	}
	
	char* buffer_from;
	char* buffer_to;
	
	buffer_from = mmap(NULL, stat_from.st_size, PROT_READ, MAP_SHARED, fd_from, 0);			//mapping source file
	if(buffer_from == (void *) -1)
	{
		perror("Error in map memory source\n");
		exit(1);
	}
	
	buffer_to =  mmap(NULL, stat_from.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_to, 0);	//mapping destination file
	if(buffer_to == (void *) -1)
	{
		perror("Error in map memory destination\n");
		exit(1);
	}
	
	if(ftruncate(fd_to, stat_from.st_size) == -1)							//resize destination file to it's new size
	{
		perror("Error in changing destination file\n");
		exit(1);
	}								
	
	buffer_to = memcpy(buffer_to, buffer_from, stat_from.st_size);					//resize destination file buffer to new destination file size
	if(buffer_to == (void *) -1)
	{
		perror("Error copying memory\n");
		exit(1);
	}
	
	munmap(buffer_from, stat_from.st_size);								//delete mapping from the specified address range
	munmap(buffer_to, stat_from.st_size);								//delete mapping from the specified address range
}

void help()
{      
	printf("This program copies one file into another\n\n");
	printf("Syntax:\n");
	printf("\t./copy [-m] <file_name> <new_file_name>\n");
	printf("\t./copy [-h]\n\n");
	
	printf("Explanation:\n");
	printf("\t-m argument\t- prints help\n");
	printf("\t-h argument\t- uses mmap()\n");
}
