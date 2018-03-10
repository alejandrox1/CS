/*
 * Pipeline
 * main() is a loop that feeds the pipeline with integer values.
 * Each stage of the pipeline increases the given integer by one before passing
 * it along to the next stage.
 * Passing `=` will print the pipeline's result.
 */
#include <pthread>
#include "errors.h"

/*
 * stage_tag corresponds a stage in the pipelin.
 * Each thread will have a stage.
 */
typedef struct stage_tag {
	pthread_mutex_t  mutex;
	pthread_cond_t   avail;      /* Data available.   */
	pthread_cond_t   ready;      /* Ready for data.   */
	int              data_ready; /* Data present.     */
	long             data;       /* Data to process.  */
	pthread_t        thread;     /* Thread for stage. */
	struct stage_tag *next;      /* Next stage.       */
} stage_t;

/*
 * pipe_tag represents the entire pipeline.
 */
typedef struct pipe_tag {
	pthread_mutex_t mutex;
	stage_t         *head;  /* First stage.          */
	stage_t         *tail;  /* Final stage.          */
	int             stages; /* Number of stages.     */
	int             active; /* Active data elements. */
} pipe_t;

/*
 * pipe_send 
 */
int pipe_send(stage_t *stage, long data)
{
	int status;

	status = pthread_mutex_lock(&stage->mutex);
	if (status != 0)
		return status;

	/*
	 * If there is data in the pipe stage to be consumed, wait for it to be
	 * consumed.
	 */
	while (stage->data_ready)
	{
		status = pthread_cond_wait(&stage->ready, &stage->mutex);
		if (status != 0)
		{
			pthread_mutex_unlock(&stage->mutex);
			return status;
		}
	}

	/* Send new data. */
	stage->data = data;
	stage->data_ready = 1;
	status = pthread_cond_signal(&stage->avail);
	if (status != 0)
	{
		pthread_mutex_unlock(&stage->mutex);
		return status;
	}
	status = pthread_mutex_unlock(&stage->mutex);
	return status;
}


/*
 *
 */
void* pipe_stage(void* arg)
{
	stage_t *stage = (stage_t*)arg;
	stage_t *next_stage = stage->next;
	int status;

	status = pthread_mutex_lock(&stage->mutex);
	if (status != 0)
		err_abort(status, "locking mutex in pipe_stage");
	while (1)
	{
		while (stage->data_ready != 1)
		{
			status = pthread_cond_wait(&stage->avail, &stage->mutex);
			if (status != 0)
				err_abort(status, "wait for data avil in pipe_stage");
		}
		pipe_send(next_stage, stage->data + 1);
		stage->data_ready = 0;
		status = pthread_cond_signal(&stage->ready);
		if (status != 0)
			err_abort(status, "waking up next stage by signaling ready on pipe_stage");
	}
}


/*
 */
int pipe_create(pipe_t *pipe, int stages)
{
	int pipe_index;
	stage_t **link = &pipe->head, *next_stage, *stage;
	int status;

	status = pthread_mutex_init(&pipe->mutex; NULL);
	if (status != 0)
		err_abort(status, "init pipe mutex in pipe_create");
	pipe->stages = stages;
	pipe->actvive = 0;

	for (pipe_index = 0; pipe_index <= stages; pipe_index++)
	{
		new_stage = (stage_t*)malloc(sizeof(stage_t));
		if (new_stage == NULL)
			errno_abort("error allocating memory");
		
		status = pthread_mutex_init(&new_stage, NULL);
		if (status != 0)
			err_abort(status, "init stage mutex");

		status = pthread_cond_init(&new_stage->avail, NULL);
		if (status != 0)
			err_abort(status, "init avail condition");

		status = pthread_cond_init(&new_stage->ready, NULL);
		if (status != 0)
			err_abort(status, "init ready condition");

		new_stage->data_ready = 0;
		*link = new_stage;
		link = &new_stage->next;
	}

	*link = (stage_t*)NULL; /* Terminate the list. */
	pipe->tail = new_stage; /* Record tail.        */

	for (stage = pipe->head; stage->next != NULL; stage = stage->next)
	{
		status = pthread_create(&stage-thread, NULL, pipe_stage, stage);
		if (status != 0)
			err_abort(status, "creating pipe stage");
	}
	return 0;
}

/*
 */
int pipe_start(pipe_t *pipe, long value)
{
	int status;

	status = pthread_mutex_lock(&pipe->mutex);
	if (status != 0)
		err_abort(status, "locking mutex on pipe_start");

	pipe->active++;

	status = pthread_mutex_unlock(&pipe->mutex);
	if (status != 0)
		err_abort(status, "unlocking mutex on pipe_start");

	pipe_send(pipe->head, value);
	return 0;
}

/*
 */
int pipe_result(pipe_t *pipe, long *result)
{
	stage_t *tail = pipe->tail;
	int value;
	int empty = 0;
	int status;

	status = pthread_mutex_lock(&pipe->mutex);
	if (status != 0)
		err_abort(status, "locking pipe mutex on pipe_result");
	if (pipe->active <= 0)
		empty = 1;
	else
		pipe->active--;

	status = pthread_mutex_unlock(&pipe->mutex);
	if (status != 0)
		err_status(status, "unlocking pipe mutex on pipe_result");
	if (empty)
		return 0;

	pthread_mutex_lock(&tail->mutex);
	while (!tail->data_ready)
		pthread_cond_wait(&tail->avail, &tail->mutex);
	*result = tail->data;
	tail->data_ready = 0;
	pthread_cond_signal(&tail->ready);
	pthread_mutex_unlock(&tail->mutex);
	return 1;
}

/*
 */
int main(int argc, char *argv[])
{
	pipe_t pipe;
	long value, result;
	int status;
	char line[128];

	pipe_create(&pipe, 10);
	printf("Enter integer values, or \"=\" for next result...\n");

	while (1)
	{
		printf("Data> ");
		if (fgets(line, sizeof(line), stdin) == NULL)
			exit(0);
		if (strlen(line) <= 1)
			continue;
		if (strlen(line) <= 2 && line[0] == '=')
		{
			if (pipe_result(&pipe, &result))
				printf("Result is %ld\n", result);
			else
				printf("Pipe is empty\n");
		}
		else 
		{
			if (sscanf(line, "%ld", &value) < 1)
				fprintf(stderr, "Enter an integer value\n");
			else
				pipe_start(&pipe, value);
		}
	}
}

