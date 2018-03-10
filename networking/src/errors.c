#ifdef _GNU_SOURCE
#undef _GNU_SOURCE
#endif
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <syslog.h>
#include "errors.h"

int daemon_proc;

/* Private functions */
static void err_doit(int errnoflag, int level, const char *fmt, va_list ap);
/*****************************************************************************
 * 								Public Functions                             *
 *****************************************************************************/
/*
 * err_sys
 */
void err_sys(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	err_doit(TRUE, LOG_ERR, fmt, ap);
	va_end(ap);

	// Handle error
	return;
}


/******************************************************************************
 * 								Private Functions                             *
 *****************************************************************************/
/*
 * err_doit
 */
static void err_doit(int errnoflag, int level, const char *fmt, va_list ap)
{
	char buf[BUFSIZE];
	char errmsg[BUFSIZE];
	int errno_save = errno;

	vsprintf(buf, fmt, ap);
	strerror_r(errno_save, errmsg, BUFSIZE);
	if (errnoflag)
		sprintf(buf+strlen(buf), ": %s", errmsg);
	strcat(buf, "\n");

	if (daemon_proc)
		syslog(level, "%s\n", buf);

	fflush(stdout);
	fputs(buf, stderr);
	fflush(stderr);
	return;
}
