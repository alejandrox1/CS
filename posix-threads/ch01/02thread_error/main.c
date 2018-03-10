#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

//#define ERRSIZE 1024 
#define ERRSIZE 4096

int main(int argc, char *argv[])
{
	pthread_t thread;
	int status;

	/*
	 * Attempt to join with an uninitialied thread ID. On most implementations,
	 * this will return an ESRCH error code. If the local (an uninitialized)
	 * pthread_t happens to be a valid thread ID, it is almost certainly that
	 * of the initial thread, which is running main(). In that case, your
	 * pthreads implementation may either return EDEADDLK (self-deadlock), or
	 * it may hang. If it hangs, quit and try again.
	 */

	status = pthread_join(thread, NULL);
	if (status != 0)
	{
		printf("Error number %d\n\n", status);

		//fprintf(stderr, "error %d: %s\n", status, strerror(status));
		char buf[ERRSIZE];
		if (strerror_r(status, buf, ERRSIZE) != 0)
			fprintf(stderr, "unkown error %d: %s\n", status, buf);
		else
			fprintf(stderr, "error %d: %s\n", status, buf);
	}	
	return status;
}
