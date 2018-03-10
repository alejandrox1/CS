#include <pthread.h>
#include <sched.h>
#include <time.h>
#include "errors.h"


typedef struct alarm_tag {
	struct alarm_tag    *link;
	int                 seconds;
	time_t              time;
	char                message[64];
} alarm_t;

pthread_mutex_t alarm_mutex = PTHREAD_MUTEX_INITIALIZER;
alarm_t *alarm_list = NULL;

void* alarm_thread(void* arg)
{
	alarm_t *alarm;
	int sleep_time;
	time_t now;
	int status;

	/*
	 * Process commands. The alarm thread will be disintegrated when process
	 * exits.
	 */
	while (1)
	{
		status = pthread_mutex_lock(&alarm_mutex);
		if (status != 0)
			err_abort(status, "lock mutex");
		alarm = alarm_list;

		/*
		 * If the alarm list is empty, wait for one second. This will allow the
		 * main thread to run and read another command.
		 * If the list is not empty, remove the first item. Compute the number
		 * of seconds to wait. If the result is less than zero (the time has
		 * passed), then set the sleep_time to 0.
		 */
		if (alarm == NULL)
			sleep_time = 1;
		else
		{
			alarm_list = alarm->link;
			now = time(NULL);
			if (alarm->time <= now)
				sleep_time = 0;
			else
				sleep_time = alarm->time - now;
#ifdef DEBUG
			printf("[waiting: %ld(%d)\"%s\"]\n", alarm->time, sleep_time, alarm->message);
#endif
		}

		/*
		 * Unlock the mutex before waiting, so that the main thread can lock it
		 * to insert a new alarm request.
		 * If the sleep time is 0, then call sched_yield, giving the main
		 * thread a chance to run if it has been readied by user input, without
		 * delaying the message if there is no input.
		 */
		status = pthread_mutex_unlock(&alarm_mutex);
		if (status != 0)
			err_abort(status, "unlock mutex");
		if (sleep_time > 0)
			sleep(sleep_time);
		else
			sched_yield();
		
		/* If a timer expired, print the message and free the structure. */
		if (alarm != NULL)
			printf("(%d) %s\n", alarm->seconds, alarm->message);
		free(alarm);
	}
	return NULL;
}



int main(int argc, char *argv[])
{
	int status;
	char line[128];
	alarm_t *alarm, *next, **last;
	pthread_t thread;

	status = pthread_create(&thread, NULL, alarm_thread, NULL);
	if (status != 0)
		err_abort(status, "create alarm thread");

	while (1)
	{
		printf("Alarm> ");
		if (fgets(line, sizeof(line), stdin) == NULL)
			exit(0);
		if (strlen(line) <= 1)
			continue;

		alarm = (alarm_t*)malloc(sizeof(alarm_t));
		if (alarm == NULL)
			errno_abort("allocating alarm");

		/*
		 * Parse input line into seconds (%d) and a message (%64[^\n])
		 * consisteing of up to 64 characters, separated from the seconds by
		 * white space.
		 */
		if (sscanf(line, "%d %64[^\n]", &alarm->seconds, alarm->message) < 2)
		{
			fprintf(stderr, "incorrect input");
			free(alarm);
		}
		else 
		{
			status = pthread_mutex_lock(&alarm_mutex);
			if (status != 0)
				err_abort(status, "locking mutex");
			alarm->time = time(NULL) + alarm->seconds;

			/*
			 * Insert the new alarm into the list of alarms, sort by expiration
			 * date.
			 */
			last = &alarm_list;
			next = *last;
			while (next != NULL)
			{
				if (next->time >= alarm->time)
				{
					alarm->link = next;
					*last = alarm;
					break;
				}
				last = &next->link;
				next = next->link;
			}
			/*
			 * If we reached the end of the list, insert the new alarm there. 
			 * "next" is NULL, and "last" points to the link field of the last
			 * item (list header).
			 */
			if (next == NULL)
			{
				*last = alarm;
				alarm->link = NULL;
			}

#ifdef DEBUG
			printf("[List: ");
			for (next = alarm_list; next != NULL; next = next->link)
				printf("%ld(%ld)[\"%s\"] ", next->time, next->time-time(NULL), next->message);
			printf("]\n");
#endif
			
			status = pthread_mutex_unlock(&alarm_mutex);
			if (status != 0)
				err_abort(status, "unlocking mutex");
		}
	}

	return 0;
}
