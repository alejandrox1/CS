#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_THREADS 10


/* This function prints the thread's identifier then exits. */
void *print_hello_world(void *tid)
{
	printf("Hello terminal! This is thread %d\n", *((int *)tid));
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	pthread_t threads[NUMBER_OF_THREADS];
	int i, status;

	for (i=0; i<NUMBER_OF_THREADS; i++)
	{
		printf("Creating thread %d\n", i);
		status = pthread_create(&threads[i], NULL, print_hello_world, (void *)&i);

		if (status != 0)
		{
			printf("Oops. pthread_create returned error code %d\n", status);
			exit(-1);
		}
	}

	exit(0);
}
