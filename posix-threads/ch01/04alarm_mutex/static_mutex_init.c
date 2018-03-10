#include <pthread.h>
#include "errors.h"

typedef struct params_tag {
	pthread_mutex_t mutex;
	int				alue;
} params_t;

params_t data = {PTHREAD_MUTEX_INITIALIZER, 0};


int main(int argc, char *argv[])
{

	return 0;
}
