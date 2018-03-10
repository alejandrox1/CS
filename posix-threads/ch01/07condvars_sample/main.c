/*
 * Simple conditional variable wait.
 */
#include <pthread.h>
#include <time.h>
#include "errors.h"

typedef struct params_tag {
	pthread_mutex_t mutex;
	pthread_cond_t  cond;
	int             value;
} params_t;

params_t data = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, 0};
int hibernation = 1;


/*
 * wait_thread is a thread start routine to set the main thread's predicate and
 * signal the condition variable.
 */
void* wait_thread(void* arg)
{
	int status;

	sleep(hibernation);
	status = pthread_mutex_lock(&data.mutex);
	if (status != 0)
		err_abort(status, "locking mutex");

	data.value = 1;
	status = pthread_cond_signal(&data.cond);
	if (status != 0)
		err_abort(status, "signaling condition");

	status = pthread_mutex_unlock(&data.mutex);
	if (status != 0)
		err_abort(status, "unlocking mutex");

	return NULL;
}



int main(int argc, char *argv[])
{
	int status;
	pthread_t wait_thread_id;
	struct timespec timeout;

	/*
	 * If a command line argument is specified, interpret it as the number of
	 * seconds for wait_thread to sleep before signaling the condition
	 * variable. 
	 */
	if (argc > 1)
		hibernation = atoi(argv[1]);


	/* Create wait thread. */
	status = pthread_create(&wait_thread_id, NULL, wait_thread, NULL);
	if (status != 0)
		err_abort(status, "creating wait thread");

	/*
	 * Wait on a conditional variable for 2 seconds, or until signaled by
	 * wait_thread.
	 * Normally, wait_thread should signal.
	 * If hibernation is greater than 2, then timeout.
	 */
	timeout.tv_sec = time(NULL) + 2;
	timeout.tv_nsec = 0;
	status = pthread_mutex_lock(&data.mutex);
	if (status != 0)
		err_abort(status, "locking mutex");

	while (data.value == 0)
	{
		status = pthread_cond_timedwait(&data.cond, &data.mutex, &timeout);
		if (status == ETIMEDOUT)
		{
			printf("condition wait timed out\n");
			break;
		}
		else if (status != 0)
			err_abort(status, "waiitng on condition");
	}

	if (data.value != 0)
		printf("condition was signaled\n");
	
	status = pthread_mutex_unlock(&data.mutex);
	if (status != 0)
		err_abort(status, "unlocking mutex");

	return 0;
}
