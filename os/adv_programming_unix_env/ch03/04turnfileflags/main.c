#include "apue.h"
#include <fcntl.h>

#define BUFFSIZE 1 //4096

/* 
 * When modifying the file descriptor or the file status flags, we must be
 * careful to fetch the existing flag value, modify it as desired, then set the
 * new flg value.
 *
 */
void
set_fl(int fd, int flags) /* Flags are file status flagsto turn on. */
{
	int val;

	if ((val = fcntl(fd, F_GETFL, 0)) < 0)
		err_sys("fcntl F_GETFL error");

	/* Turn on flags. */
	val |= flags;
	/* Turn off flags. */
	//val &= ~flags;

	if (fcntl(fd, F_SETFL, val) < 0)
		err_sys("fcntl F_SETFL error");
}


int 
main(void)
{
	int  n;
	char buf[BUFFSIZE];

	set_fl(STDOUT_FILENO, O_SYNC);

	while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0)
		if (write(STDOUT_FILENO, buf, n) != n)
			err_sys("write error");

	if (n < 0)
		err_sys("read error");

	exit(0);
}


