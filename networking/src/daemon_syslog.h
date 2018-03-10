#ifndef __DAEMON_SYSLOG_H__
#define __DAEMON_SYSLOG_H__

extern int daemon_proc;

void syslog_init(const char *pname, int facility);
void syslog_term();

#endif // __DAEMON_SYSLOG_H__

