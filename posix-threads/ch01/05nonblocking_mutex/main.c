#include <pthread.h>
#include "errors.h"

#define SPIN 10000000

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
long counter;
time_t end_time;

/*
 * counter_thread is a start routine that will repeatedly lock the mutex and
 * increment the counter.
 */
void* counter_thread(void* arg)
{
	int status;
	int spin;

	/*
	 * Increment counter each second until end_time.
	 * Instrad of simply ncremeenting the counter, it sleeps for another second
	 * with the mutex still locked, to give the monitor_thread more time to
	 * run.
	 */
	while (time(NULL) <end_time)
	{
		status = pthread_mutex_lock(&mutex);
		if (status != 0)
			err_abort(status, "locking mutex");
		
		for (spin=0; spin<SPIN; spin++)
			counter++;

		status = pthread_mutex_unlock(&mutex);
		if (status != 0)
			err_abort(status, "unlocking mutex");
		sleep(1);
	}
	printf("Counter is %#lx\n", counter);
	return NULL;
}


/*
 * monitor_thread is a start routine to monitor the counter.
 * Every 3 seconds it will try to lock the mutex and read the counter.
 * If the trylock fails it will just skip a cycle.
 */
void* monitor_thread(void* arg)
{
	int status;
	int misses = 0;

	/* Loop until end_time, check counter every 3 seconds. */
	while (time(NULL) < end_time)
	{
		sleep(3);
		
		status = pthread_mutex_trylock(&mutex);
		if (status != EBUSY)
		{
			if (status != 0)
				err_abort(status, "try locking mutex");
			
			printf("Counter is %ld\n", counter/SPIN);

			status = pthread_mutex_unlock(&mutex);
			if (status != 0)
				err_abort(status, "unlocking mutex");
		}
		else
			misses++;
	}
	printf("Monitor thread missed %d updates\n", misses);
	return NULL;
}



int main(int argc, char *argv[])
{
	int status;
	pthread_t counter_thread_id, monitor_thread_id;

	end_time = time(NULL) + 120;
	
	status = pthread_create(&counter_thread_id, NULL, counter_thread, NULL);
	if (status != 0)
		err_abort(status, "creating counter thread");
	status = pthread_create(&monitor_thread_id, NULL, monitor_thread, NULL);
	if (status != 0)
		err_abort(status, "creating monitor thread");


	status = pthread_join(counter_thread_id, NULL);
	if (status != 0)
		err_abort(status, "joining counter thread");
	status = pthread_join(monitor_thread_id, NULL);
	if (status != 0)
		err_abort(status, "joining monitor thread");

	return 0;
}
