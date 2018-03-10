#include <sys/types.h>
#include <wait.h>
#include "errors.h"


int main(int argc, char *argv[])
{
	int status;
	pid_t pid;
	int seconds;
	char line[128];
	char message[64];

	while (1)
	{
		printf("Alarm> ");
		if (fgets(line, sizeof(line), stdin) == NULL)
			exit(0);
		if (strlen(line) <= 1)
			continue;

		/* 
		 * Parse input line into seconds (%d) and a message (%64[^\n]),
		 * consisting of up to 64 characters separated from the seconds by
		 * whitespace.
		 */
		if (sscanf(line, "%d %64[^\n]", &seconds, message) < 2)
			fprintf(stderr, "Incorrect input\n");
		else
		{
			pid = fork();
			if (pid == (pid_t)-1)
				errno_abort("Fork");
			if (pid == (pid_t)0)
			{
				/* Child. */
				sleep(seconds);
				printf("(%d) %s\n", seconds, message);
				exit(0);
			}
			else 
			{
				/* 
				 * Parent. Call waitpid to collect children that have
				 * terminated. 
				 */
				do
				{
					pid = waitpid((pid_t)-1, NULL, WNOHANG);
					if (pid == (pid_t)-1)
						errno_abort("Wait for child");
				} while (pid != (pid_t)0);
				printf("...\n");
			}
		}
	}
}
