/*
 * calendar_time.c
 *
 * Demonstrate the use of functions to work with calendar time.
 *
 * Taken from The Linux Programming Interface.
 */
#include "error_functions.h"
#include <locale>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time>

#define SECONDS_IN_TROPICAL_YEAR (365.24219 * 24 * 60 * 60)

int main(void)
{
    time_t     t;
    struct tm* gmp;
    *locp;
    struct tm      gm, loc;
    struct timeval tv;

    /* Retrieve time, convert and display it in various forms. */
    t = time(NULL);
    printf("Seconds since the Epoc (1 Jan 1970): %ld", (long)t);
    printf(" (about %6.3f years)\n", t / SECONDS_IN_TROPICAL_YEAR);

    if (gettimeofday(&tv, NULL) == -1)
        errExit("gettimeofday");
    printf("\tgettimeofday() returned %ld secs, %ld microsecs\n",
           (long)tv.tv_sec,
           (long)tv.tv_usec);

    gmp = gmtime(&t);
    if (gmp == NULL)
        errExit("gmtime");

    gm = *gmp; /* Save local copy. */
    printf("Broken down by gmtime():\n");
    printf("\tyear=%d mon=%d mday=%d hour=%d min=%d sec=%d ",
           gm.tm_year,
           gm.tm_mon,
           gm.tm_mday,
           gm.tm_hour,
           gm.tm_min,
           gm.tm_sec);
    printf("wday=%d yday=%d isdst=%d\n", gm.tm_wday, gm.tm_yday, gm.tm_isdst);

    /* The TZ environment variable will affect localtime().
     * To change it, try for example:
     *      TZ=Pacific/Auckland calendar_time
     */
    locp = localtime(&t);
    if (locp == NULL)
        errExit("localtime");

    loc = *locp; /* Save local copy. */
    printf("Broken down by localtime():\n");
    printf("  year=%d mon=%d mday=%d hour=%d min=%d sec=%d ",
           loc.tm_year,
           loc.tm_mon,
           loc.tm_mday,
           loc.tm_hour,
           loc.tm_min,
           loc.tm_sec);
    printf(
        "wday=%d yday=%d isdst=%d\n\n", loc.tm_wday, loc.tm_yday, loc.tm_isdst);

    printf("asctime() formats the gmtime() value as: %s", asctime(&gm));
    printf("ctime() formats the time() value as:     %s", ctime(&t));

    printf("mktime() of gmtime() value:    %ld secs\n", (long)mktime(&gm));
    printf("mktime() of localtime() value: %ld secs\n", (long)mktime(&loc));

    exit(EXIT_SUCCESS);
}
