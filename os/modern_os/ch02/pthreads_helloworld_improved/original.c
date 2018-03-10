#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_THREADS 10

struct params {
	pthread_mutex_t mutex;
	int id;
};

typedef struct params params_t;

void* hello(void *arg)
{
	int id;

	pthread_mutex_lock(&(*(params_t*)(arg)).mutex);
	
	id = (*(params_t*)(arg)).id;
	printf("Hello from %d\n", id);
		
	pthread_mutex_unlock(&(*(params_t*)(arg)).mutex);

	return NULL;
}

int main()
{
	pthread_t threads[NUMBER_OF_THREADS];
	params_t params;
	pthread_mutex_init (&params.mutex, NULL);

	int i, status;
	for (i=0; i<NUMBER_OF_THREADS; i++)
	{
		params.id = i;
		status = pthread_create(&threads[i], NULL, hello, &params);
		if (status != 0)
			exit(1);
	}

	for (i=0; i<NUMBER_OF_THREADS; i++)
		pthread_join(threads[i], NULL);

	exit(0);
}
