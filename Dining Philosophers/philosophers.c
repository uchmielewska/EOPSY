#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <signal.h>
#include <wait.h>

#define N 5
#define LEFT PHILO_ID
#define RIGHT (PHILO_ID + 1) % N

//synchronization actions
void grab_forks(int left_fork_id);
void put_away_forks(int left_fork_id);

//philosophers actions
void eat(int i);
void *philosopher();
void child_terminate(int i);

int philos_pids[N];
int philo_id;
static int SEMID;

int main(void)
{
	//philosophers initialization
	SEMID = semget(IPC_PRIVATE, N, 0666);		//IPC_PRIVATE - so new set of nsems semaphores is created
	if(SEMID < 0)					//SEMID = -1 denoted failure in creating new semaphore
	{
		perror("Error with semget\n");
		exit(1);
	}


	for(int i = 0; i < N; i++)				//set all semaphores to 1 (all forks are on the table initially)
	{	
		if(semctl(SEMID, i, SETVAL, 1) == -1)		//control operations (semid to identify, setval -control operation, set the sem value
		{
			perror("Error with semctl\n");
			exit(1);
		}
	}
	
	
	pid_t child;						//pid_t is a data type for process identification id
	int child_counter;
	for(child_counter = 0; child_counter < N; child_counter++)
	{
		child = fork();					//fork() creates new process - child creation
		
		if(child == -1)
		{
			printf("Creating child process failed\n");
			child_terminate(child_counter);
			exit(1);
		} // if child == 0 were in child process
		else if(child == 0)
		{
			philo_id = child_counter;
			philos_pids[child_counter] = getpid();
			philosopher(child_counter);
			exit(0);
		}
		//if child > 0 parent process so continue iteration
	}
	
	int status;
	for(child_counter = 0; child_counter < N; child_counter++)
	{
		child = wait(&status);
		
		if(child == -1)
		{
			break;
		}
	}
	if(semctl(SEMID, 0, IPC_RMID) == -1) 
	{
		printf("Error with removing semaphore\n");
		exit(1);
	}
	
	return 0;
}


void grab_forks(int left_fork_id)
{
	int PHILO_ID = left_fork_id;
	
	struct sembuf sem_check[] = 			//because semop requires struct sembuf as parameter
	{
		{ .sem_num = LEFT, .sem_op = -1, .sem_flg = 0 },	//lock fork
		{ .sem_num = RIGHT, .sem_op = -1, .sem_flg = 0 },	//lock fork
	};
	
	if(semop(SEMID, sem_check, 2) == -1){
		perror("grab_forks(): error with semop\n");
		exit(1);
	}
}


void put_away_forks(int left_fork_id)
{
	int PHILO_ID = left_fork_id;
	
	struct sembuf sem_check[] = 			//because semop requires struct sembuf as parameter
	{
		{ .sem_num = LEFT, .sem_op = 1, .sem_flg = 0 },	//unlock fork	
		{ .sem_num = RIGHT, .sem_op = 1, .sem_flg = 0 },	//unlock fork
	};
	
	if(semop(SEMID, sem_check, 2) == -1)
	{
		perror("pu_away_forks(): error with semop\n");
		exit(1);
	}
	
	printf("Philosopher[%d] puts away forks [%d] and [%d]\n", RIGHT, LEFT, RIGHT);
	printf("Philosopher[%d] is thinking\n", RIGHT);
}


void eat(int i)
{
	int PHILO_ID = i;			//initialide LEFT and RIGHT
	sleep(2);
	printf("Philosopher[%d] grabs forks %d and %d\n", RIGHT, LEFT, RIGHT);
	printf("Philosopher[%d] is eating\n", RIGHT);
}


void* philosopher()
{
	int PHILO_ID = philo_id;			//set id of the current philosopher
	
	while(1)
	{
		printf("Philosopher[%d] is thinking\n", philo_id);
		sleep(1);
		grab_forks(philo_id);
		eat(philo_id);
		sleep(1);
		put_away_forks(philo_id);
	}
}

void child_terminate(int i)
{
	int j = 0;
	for(j = 0; j < i; j++)
	{
		kill(philos_pids[j], SIGTERM);
	}
}
