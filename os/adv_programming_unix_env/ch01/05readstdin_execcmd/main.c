#include "apue.h"
#include <sys/wait.h>


int main(int argc, char *argv[]){
	char  buf[MAXLINE]; 
	pid_t pid;
	int   status;

	printf("%% ");
	while (fgets(buf, MAXLINE, stdin) != NULL) {
		if (buf[strlen(buf)-1] == '\n') {
			// Replace newline with null for execlp.
			buf[strlen(buf)-1] = 0;
		}
	
		/* Call fork to createa new process, which is a copy of the caller. The
		 * caller is the parent, newly createdprocess the child.
		 * Subsequently, fork returns the non-negative process ID of the new
		 * child process to the parent, and returns 0 to the child.
		 */
		if ((pid = fork()) < 0) {
			err_sys("fork error");
		} else if (pid == 0) {           // Child.
			// Replaces child process with new program file.
			execlp(buf, buf, (char*)0);
			err_ret("couldn't execute: %s", buf);
			exit(127);
		}

		// Parent.
		if ((pid = waitpid(pid, &status, 0)) < 0)
			err_sys("waitpid error");

		printf("%% ");
	}
	exit(0);
}
