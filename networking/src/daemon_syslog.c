#include <syslog.h>
#include "daemon_syslog.h"


/*
 * syslog_init initiates the connection to the system logger.
 * For more info on facility options see syslog(3).
 */
void syslog_init(const char *pname, int facility)
{
	daemon_proc = 1;
	openlog(pname, LOG_PID, facility);
}

/*
 * syslog_term closes the connection to the system logger.
 */
void syslog_term()
{
	closelog();
}
