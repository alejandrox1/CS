#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUMTHREADS 10

typedef struct params {
	int id;
} params_t;


void* hello(void *arg)
{
	params_t *p = (params_t*)arg;
	int status;

	status = pthread_detach(pthread_self());
	if (status !=0 )
	{
		printf("detaching thread\n");
		abort();
	}

	printf("Hello from %d\n", p->id);
	free(p);
	return NULL;
}

int main()
{
	pthread_t thread;
	params_t *par;

	int i, status;
	for (i=0; i<NUMTHREADS; i++)
	{
		par = (params_t*)malloc(sizeof(params_t));
		if (par == NULL)
		{
			printf("allocating params_t");
			abort();
		}

		par->id = i;
		status = pthread_create(&thread, NULL, hello, par);
		if (status != 0)
			exit(1);
	}

	sleep(5);

	exit(0);
}
