/*
 * Use mutexes to synchroniz access to the alarm list.
 * The alarm thread waits on a condiional variable, with a timeout
 * corresponding to the earliest timer request.
 * If the main thread enters an earlier timeout, it signals the condition
 * variable to wake up the alarm thread will wake up and process the earlier
 * timeout first, requeueing the later request.
 */
#include <pthread.h>
#include <time.h>
#include "errors.h"

typedef struct alarm_tag {
	struct alarm_tag *link;
	int              seconds;
	time_t           time;
	char             message[64];
} alarm_t;

pthread_mutex_t alarm_mutex   = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  alarm_cond    = PTHREAD_COND_INITIALIZER;
alarm_t         *alarm_list   = NULL;
time_t          current_alarm = 0;

/* alarm_inset will insert an alarm request in order into the alarm list. */
void alarm_insert(alarm_t* alarm)
{
	int status;
	alarm_t *next, **last;

	/*
	 * Locking protocol:
	 * This routine requires the caller have locked the alarm mutex.
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
	 * If we have reached the end of the list, insert the new alarm there
	 * (next is null, and last points to the link field of the last item, the
	 * list header).
	 */
	if (next == NULL)
	{
		*last = alarm;
		alarm->link = NULL;
	}

#ifdef DEBUG
	printf ("[list: ");                                                         
	for (next = alarm_list; next != NULL; next = next->link)  
		printf ("%ld(%ld)[\"%s\"] ", next->time, next->time-time(NULL), next->message);                           
	printf ("]\n");                                                             
#endif
	/*
	 * Wake the alarm thread if its not busy (if current_alarm is set to 0,
	 * which means it is waiting for work), or if the new alarm comes before
	 * the one on which the alarm thread is waiting.
	 */
	if (current_alarm==0 || current_alarm>alarm->time)
	{
		current_alarm = alarm->time;
		status = pthread_cond_signal(&alarm_cond);
		if (status != 0)
			err_abort(status, "signaling cond");
	}
}


/* alarm_thread is the start routine for the thread. */
void* alarm_thread(void* arg)
{
	alarm_t *alarm;
	struct timespec cond_time;
	time_t now;
	int status, expired;

	/*
	 * Alarm thread will be disintegrated when process exits.
	 * Lock mutex at start - it will be unlocked during condition waits so that
	 * the main thread can can insert data.
	 */
	status = pthread_mutex_lock(&alarm_mutex);
	if (status != 0)
		err_abort(status, "locking mutex");

	while (1)
	{
		/*
		 * If alarm_list is empty then wait until an alarm request is added. 
		 * Setting current_alarm to 0 informs the insert routine that the
		 * thread is not busy.
		 */
		current_alarm = 0;
		while (alarm_list == NULL)
		{
			status = pthread_cond_wait(&alarm_cond, &alarm_mutex);
			if (status != 0)
				err_abort(status, "waiting on condition");
		}

		alarm = alarm_list;
		alarm_list = alarm->link;
		now = time(NULL);
		expired = 0;
		if (now < alarm->time)
		{
#ifdef DEBUG
			printf ("[waiting: %d(%d)\"%s\"]\n", alarm->time, alarm->time-time(NULL), alarm->message);
#endif
			cond_time.tv_sec = alarm->time;
			cond_time.tv_nsec = 0;
			current_alarm = alarm->time;
			while (current_alarm == alarm->time)
			{
				status = pthread_cond_timedwait(&alarm_cond, &alarm_mutex, &cond_time);
				if (status == ETIMEDOUT)
				{
					expired = 1;
					break;
				}
				if (status != 0)
					err_abort(status, "condition timed wait");
			}
			if (!expired)
				alarm_insert(alarm);
		}
		else
			expired = 1;

		if (expired)
		{
			printf("(%d) %s\n", alarm->seconds, alarm->message);
			free(alarm);
		}
	}
}


int main (int argc, char *argv[])                                               
{ 
	int status;
	char line[128];
	alarm_t *alarm;
	pthread_t thread;
	
	status = pthread_create(&thread, NULL, alarm_thread, NULL);
	if (status != 0)
		err_abort (status, "Create alarm thread");                              
    
	while (1) {
		printf ("Alarm> ");
		if (fgets (line, sizeof(line), stdin) == NULL) 
			exit(0);
		if (strlen(line) <= 1) 
			continue;                                       
		
		alarm = (alarm_t*)malloc(sizeof(alarm_t));
		if (alarm == NULL)
			errno_abort ("Allocate alarm"); 
		
		/* 
		 * Parse input line into seconds (%d) and a message (%64[^\n]), 
		 * consisting of up to 64 characters separated from the seconds by 
		 * whitespace.
		 */
		if (sscanf(line, "%d %64[^\n]", &alarm->seconds, alarm->message) < 2) 
		{   
			fprintf (stderr, "Bad command\n"); 
			free (alarm);
		} 
		else 
		{
			status = pthread_mutex_lock(&alarm_mutex);
			if (status != 0)
				err_abort(status, "Lock mutex");  
			
			/* 
			 * Insert the new alarm into the list of alarms, sorted by 
			 * expiration time.  
			 */
			alarm->time = time(NULL) + alarm->seconds; 
			alarm_insert(alarm);

			status = pthread_mutex_unlock(&alarm_mutex); 
			if (status != 0)
				err_abort(status, "Unlock mutex");
		}
	}
}
