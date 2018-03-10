/*
 * Adapted from:
 * https://www.geeksforgeeks.org/petersons-algorithm-for-mutual-exclusion-set-1/
 *
 * 	A thread expresses its desired to acquire the lock and sets interested[self] = 1,
 * 	then gives other threads a chance to acquire the lock.
 *
 * If a thread desires to acqurie the lock, then it gets the lock and
 * consecuently gives a chance to other threads.
 * If a thread does not desire lock then it breaks from a loop and life goe on.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "mythreads.h"


#define FALSE 0
#define TRUE  1
#define NUMBER_OF_THREADS 2


const int MAX = 1e5;
int ans = 0;
int turn;
int interested[NUMBER_OF_THREADS];

void lock_init();
void lock(int self);
void unlock(int self);
void* func(void* s);


int main()
{
	// Initialized lock then fork N threads.
	pthread_t threads[NUMBER_OF_THREADS];
	lock_init();

	int i, status;
	for (i=0; i<NUMBER_OF_THREADS; i++)
	{
		status = pthread_create(&threads[i], NULL, func, (void*)&i);
		if (status != 0)
		{
			printf("Error creating thread. Got error code %d\n", status);
			exit(1);
		}
	}
	
	for (i=0; i<NUMBER_OF_THREADS; i++)
		pthread_join(threads[i], NULL);

	printf("Actual count: %d | Expec count: %d\n", ans, MAX*NUMBER_OF_THREADS);

	exit(0);
}

// Initialize lock by reseting the desire of all both threads to acquire the
// lock and give the turn to one of them.
void lock_init()
{

	int i;
	for (i=0; i<NUMBER_OF_THREADS; i++)
		interested[i] = FALSE;
	turn = 0;
}


// Execute before ntering critical region.
void lock(int process)
{
	// Seting flag to 1 indicates desire to acquire lock.
	interested[process] = TRUE;

	// Give over thread a chance to acquire lock.
	turn = process;

	// Wait until other thread loses desire to acquire lock until it is your
	// turn.
	while (turn==process && interested[1-process]==TRUE);
}


// Execute after leaving critical region.
void unlock(int process)
{
	// Allow other threads to acquire lock.
	interested[process] = FALSE;
}


void* func(void* s)
{
	int i;
	//int self = *(int*)s;
	
	printf("Thread %d entered...\n", self);

	lock(self);

	for (i=0; i<MAX; i++)
		ans++;

	unlock(self);
	return NULL;
}
