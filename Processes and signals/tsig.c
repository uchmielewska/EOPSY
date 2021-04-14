#define _POSIX_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <stdbool.h>

#define NUM_CHILD 7 				//number of child processes to be created
#define WITH_SIGNALS				//indicator if the algorithm should be run with or without signal handlers

void create_child(int);			//def. function to create single child process
void kill_children(int, int*);		//def. function to kill all already created children processes

pid_t children_ids[NUM_CHILD];		//array with children ids
int children_count = 0;			//children counter

int main () {
	
	int i;					//current child process counter
	int child_terminations = 0;		//children processes counter
	
	printf("parent[%d]: process created\n", getpid());

	for (i = 0; i < NUM_CHILD; i++){
		create_child(i);		//whole creating child process is in create_child function
		sleep(1);			//1s delay between consecutive fork() calls
	}
	printf("parent[%d]: all child processes created with success\n", getpid());
	 
	while (1){				//infinite loop which breaks if all children were already processed
		if(wait(NULL) == -1)		//if any process has no child process - wait() returns -1
			break;
		else
			child_terminations++;	//children process counter
	}
	printf("parent[%d]: no more kids to be processed\n", getpid());
	printf("parent[%d]: terminated %d children processes\n", getpid(), child_terminations);
}

void create_child(int i){
	pid_t child_id;			//process id
	child_id = fork();			//create new child process
	
	if (child_id < 0) {			//uncessful child creation
		printf("parent[%d]: child process creation failed, killing all children and terminating program\n", getpid());
		kill_children(i, children_ids);
		exit(1);
	} else if (child_id == 0) {		//successfull new child creation process
		printf("parent[%d]: created child(%d)\n",getppid(), getpid());
		sleep(10);			//10s delay
		printf("child[%d]: execution completed\n",getpid());
		exit(0);
	} else {
		children_ids[i] = child_id;	//save child_id to array
		children_count++;
	}
}

void kill_children(int num_child, int* children){
	for (int j = 0; j < num_child; j++){
		kill(children[j], SIGTERM);
	}
}
