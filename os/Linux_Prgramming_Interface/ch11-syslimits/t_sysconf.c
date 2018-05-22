/*
 * t_sysconf.c
 *
 * Demonstrate the use of sysconf() to retrieve system limits.
 *
 * Taken from The Linux Programming Interface.
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "error_functions.h"

static void sysconfPrint(const char* msg, int name);

int main(void)
{
    sysconfPrint("_SC_ARG_MAX:        ", _SC_ARG_MAX);
    sysconfPrint("_SC_LOGIN_NAME_MAX: ", _SC_LOGIN_NAME_MAX);
    sysconfPrint("_SC_OPEN_MAX:       ", _SC_OPEN_MAX);
    sysconfPrint("_SC_NGROUPS_MAX:    ", _SC_NGROUPS_MAX);
    sysconfPrint("_SC_PAGESIZE:       ", _SC_PAGESIZE);
    sysconfPrint("_SC_RTSIG_MAX:      ", _SC_RTSIG_MAX);
    exit(EXIT_SUCCESS);
}


static void sysconfPrint(const char* msg,int name)
{
    long lim;

    errno = 0;
    lim = sysconf(name);
    if (lim != -1)            // Call succeded, limit determinate.
    {
        printf("%s %ld\n", msg, lim);
    }
    else
    {
        if (errno == 0)       // Call succeeded, limit indeterminate.
            printf("%s (indeterminate)\n", msg);
        else                  // Call failed.
            errExit("sysconf %s", name);
    }
}
