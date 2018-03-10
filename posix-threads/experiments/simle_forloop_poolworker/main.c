#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct params_tag {
	int sockfd;
} params_t;

pthread_t threads[3];

void* handler(void* arg)
{
	params_t *params = (params_t *)arg;

	printf("val %d from %ld\n", params->sockfd, pthread_self());
	free(params);
	return NULL;
}


int main()
{
	int tid=0;

	int i, sd;
	for (i=0; i<10; i++)
	{
    	sd = i;
		int j;
		for (j=0; j<1; j++)
		{
			params_t *params = (params_t *)malloc(sizeof(params_t));
			params->sockfd = sd;
			pthread_create(&threads[tid], NULL, handler, params);
			tid = (tid+1)%3;
		}
	}

	for (i=0; i<3; i++)
		pthread_join(threads[i], NULL);
	return 0;
}
