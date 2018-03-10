#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 21
#define NUMTHREADS 5

int numbers[N] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

struct params
{
	size_t start, end;
	pthread_t tid;
	pthread_mutex_t mutex;                                                      
	pthread_cond_t done;
};

typedef struct params params_t;

void* method(void*);

int main()
{
	pthread_t threads[NUMTHREADS];                                       
	params_t params;                                                            
	pthread_mutex_init (&params.mutex, NULL);                                   
	pthread_cond_init (&params.done, NULL);                                    
	// Obtain a lock on the parameter.                                          
	pthread_mutex_lock(&params.mutex);
	
	size_t idStart = 0, idEnd, idn = N/NUMTHREADS;
	idEnd = idStart + idn;

	int i, status;
	printf("num of iters %ld\n", idn);
	for (i=0; i<=NUMTHREADS; i++)
	{
		params.start = idStart;
		params.end = idEnd;
		//printf("(%ld, %ld)\n", idStart, idEnd);

		status = pthread_create(&threads[i], NULL, method, &params);
		if (status != 0)
			exit(1);
		pthread_cond_wait(&params.done, &params.mutex);

		idStart = idEnd;
		idEnd = (idEnd+idn < N ? idEnd+idn : N);
	}

	pthread_mutex_destroy(&params.mutex);                                       
	pthread_cond_destroy(&params.done); 

	exit(0);
}

void* method(void* arg)
{
	int i, start, end;                                                                      
	pthread_mutex_lock(&(*(params_t*)(arg)).mutex);

	start = (*(params_t*)(arg)).start;
	end = (*(params_t*)(arg)).end;
	for (i=start; i<end; i++)
		printf("%d ", numbers[i]);
	printf("\n");

	pthread_mutex_unlock(&(*(params_t*)(arg)).mutex);                           
	pthread_cond_signal(&(*(params_t*)(arg)).done);
	//fflush(stdout);
	return NULL;
}
