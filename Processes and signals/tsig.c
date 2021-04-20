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

void create_child(int);			//func. function to create single child process

pid_t children_ids[NUM_CHILD];		//array with children ids
int children_count = 0;			//children counter
bool keyboard_interrupt = false;		//variable to notify about keyboard interrupt

#ifdef WITH_SIGNALS
void kill_children(int);			//func. function to kill all already created children processes
void sigint_handler();				//func. keyboard interrupt handler
void term_child(int);				//func. information about terminating child
#endif

int main() {
	
	int i;					//current child process counter
	int child_terminations = 0;		//children processes counter
	
	#ifdef WITH_SIGNALS
	for(i = 1; i < _NSIG; ++i){
		signal(i,SIG_IGN);		//ignoring signals
	}
	
	signal(SIGCHLD,SIG_DFL);		//reset SIGCHLD to default
	signal(SIGINT,sigint_handler);	//my own keyboard interrupt handler
	#endif
	
	printf("parent[%d]: process created\n", getpid());

	for (i = 0; i < NUM_CHILD; ++i){
		create_child(i);		//whole creating child process is in create_child function
		sleep(1);			//1s delay between consecutive fork() calls
		
		#ifdef WITH_SIGNALS
		if(keyboard_interrupt){
			printf("parent[%d]: sending SIGTERM signal to all already created children...\n",getpid());
			kill_children(i);
			break;
		}
		#endif
	}
	
	if(!keyboard_interrupt){
		printf("parent[%d]: all child processes created with success\n", getpid());
	}
		
	while (1){				//infinite loop which breaks if all children were already processed
		if(wait(NULL) == -1)		//if any process has no child process - wait() returns -1
			break;
		else
			child_terminations++;	//children process counter
	}
	
	printf("parent[%d]: no more kids to be processed\n", getpid());
	printf("parent[%d]: terminated %d children processes\n", getpid(), child_terminations);

	#ifdef WITH_SIGNALS
	for(i = 1; i < _NSIG; ++i){
		signal(i, SIG_DFL);		//restore default service handlers
	}
	#endif
}

//functions
void create_child(int i){
	pid_t child_id;			//process id
	child_id = fork();			//create new child process
	
	if (child_id < 0){			//uncessful child creation
		printf("parent[%d]: child creation failed\n", getpid());
	
		#ifdef WITH_SIGNALS
		kill_children(i);
		#endif
		exit(1);
		
	} else if (child_id == 0){		//successfull new child creation process
	
		#ifdef WITH_SIGNALS
		signal(SIGINT,SIG_IGN);	//ignore keyboard interrupt
		signal(SIGTERM,term_child);	//terminating process handler
		#endif
		
		printf("parent[%d]: created child(%d)\n",getppid(), getpid());
		sleep(10);			//10s delay
		printf("child[%d]: execution completed\n",getpid());
		exit(0);
	} else{
		children_ids[i] = child_id;	//save child_id to array
		children_count++;
	}
}

#ifdef WITH_SIGNALS
void kill_children(int num_child){
	for (int j = 0; j <= num_child; ++j){
		printf("parent[%d]: sending SIGTERM to child[%d]\n",getpid(), children_ids[j]);
		kill(children_ids[j], SIGTERM);
	}
}

void sigint_handler(){
	printf("parent[%d]: keyboard interruption detected\n",getpid());
	keyboard_interrupt = true;
}

void term_child(int signal)
{
	printf("child[%d]: I'm beeing terminated\n",getpid());
	exit(1);
}
#endif
