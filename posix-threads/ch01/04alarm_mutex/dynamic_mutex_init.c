#include <pthread.h>
#include "errors.h"

typedef struct params_tag {
	pthread_mutex_t mutex;
	int				value;
} params_t;


int main(int argc, char *argv[])
{
	params_t *data;
	int status;

	data = (params_t*)malloc(sizeof(params_t));
	if (data == NULL)
		errno_abort("allocate structure");

	status = pthread_mutex_init(&data->mutex, NULL);
	if (status != 0)
		err_abort(status, "init mutex");

	status = pthread_mutex_destroy(&data->mutex);
	if (status != 0)
		err_abort(status, "mutex destory");

	free(data);
	return 0;
}
