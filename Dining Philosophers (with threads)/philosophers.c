#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>


#define N 5
#define LEFT (N + id - 1) % N
#define RIGHT (id + 1) % N
#define ITERATIONS 3

#define THINKING 0
#define HUNGRY 1
#define EATING 2


pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;		//forks mutex
pthread_mutex_t s[N] = PTHREAD_MUTEX_INITIALIZER;		//philosophers mutex
int state[N];


void grab_forks(int left_fork_id);
void put_away_forks(int left_fork_id);
void* philosopher(void* num);
void test(int id);
void eat(int i, int meal_it);


int main()
{
   
   pthread_t thread_ids[N];			//initialize array of threads for philos
   int philos_ids[N];				//initialize id's array for philos
   
   
   for(int i = 0; i < N; i++)
   {
   	pthread_mutex_init(&s[i], NULL);		//init makes unlocked mutex
   	pthread_mutex_lock(&s[i]);			//lock philos mutexes = initialize with 0
   }
   
   //initialize threads
   for(int i = 0; i < N; i++)
   {
   	philos_ids[i] = i;
   	
   	//create thread
   	pthread_create(thread_ids + i, NULL, philosopher, philos_ids +i);		//start new thread by calling 'philosopher' routine (stored new thread id in bufer 'philos'

   }	
   
   sleep(30);		//wait while threads are running
   
   //join threads
   for(int i = 0; i < N; i++)
   {	
   	pthread_cancel(thread_ids[i]);
   
   	if(pthread_join(thread_ids[i], NULL) != 0)
   	{
   		perror("Error in killing threads\n");
   		exit(1);
   	}	
   }
   

   for(int i = 0; i < N; i++)
   {
   	pthread_mutex_destroy(&s[i]);	//philos mutexes destroy
   }

    return 0;
}


void *philosopher(void *num)
{
    //Set the ID of the current philosopher
    int id = *(int *) num;

    for (int i = 0; i < ITERATIONS; i++) 
    {
            printf("Philosopher[%d] is thinking\n", id);
            sleep(1);
            grab_forks(id);               	    		 //Philosopher grabs forks
            eat(id, i+1);                            	//Philosopher is eating
            sleep(1);
	    put_away_forks(id);               	 //Philosopher puts forks down
    }

        return NULL;

}



void grab_forks(int left_fork_id)
{
    int id = left_fork_id;

    pthread_mutex_lock(&m); 				//lock mutex - begin Critical Section
    state[id] = HUNGRY;

    printf("Philosopher[%d] wants to eat.\n", RIGHT);
    test(id);

    pthread_mutex_unlock(&m);   			//unlock mutex - end of ciritical section

    pthread_mutex_lock(&s[id]); 			//take forks

    sleep(1);
}

void test(int i)
{
    int id = i;

    if( state[id] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING )
    {
        state[id] = EATING;
        pthread_mutex_unlock(&s[id]);
    }
}

void put_away_forks(int left_fork_id)
{
    int id = left_fork_id;

    pthread_mutex_lock(&m);            	//begin Critical Section
    state[id] = THINKING;  			//set state to the philosopher to THINKING

    printf("Philosopher[%d] finished his meal and is thinking.\n", RIGHT);

    test(LEFT);		                    //Test eating conditions for the left philosopher
    test(RIGHT);	                	    //Test eating conditions for the right philosopher
    pthread_mutex_unlock(&m); 		    //end Critical Section
}


void eat(int i, int meal_it)
{
    int id = i;                         //initialization for defined LEFT and RIGHT

    printf("Philosopher[%d] is eating [%d] meal\n", RIGHT, meal_it);
}
