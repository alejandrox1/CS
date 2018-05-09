/*
 * strtime.c
 *
 * Demonstrate the use of strftime() and strptime().
 *
 * Call strptime() using a given format to process the date+time input.
 * The conversion is then reversed by calling strftime(), which is supplied
 * with an output format, or a default if none is provided.
 *
 * Example:
 *      ./strtime "9:39:46pm 1 Feb 2011" "%I:%M:%S%p %d %b %Y"
 *
 *      ./strtime "9:39:46pm 1 Feb 2011" "%I:%M:%S%p %d %b %Y" "%F %T"
 *
 * Taken from The Linux Programming Interface.
 */
#if ! defined(__sun)
# ifndef _XOPEN_SOURCE
# define _XOPEN_SOURCE
# endif
#endif
#include <locale.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error_functions.h"

#define SBUF_SIZE 1000

int main(int argc, char* argv[])
{
    struct tm tm;
    char sbuf[SBUF_SIZE];
    char* ofmt;

    if (argc < 3 || strcmp(argv[1], "--help") == 0)
        usageError("%s input-date-time in-format [out-format]\n", argv[0]);

    if (setlocale(LC_ALL, "") == NULL)
        errExit("setlocale");

    memset(&tm, 0, sizeof(struct tm));
    if (strptime(argv[1], argv[2], &tm) == NULL)
        fatal("strptime");

    /* Not set by strptime(); tells mktime() to determine if DST is in effect. */
    tm.tm_isdst = -1;
    
    printf("calendar time (seconds since Epoch): %ld\n", (long)mktime(&tm));

    ofmt = (argc > 3) ? argv[3]: "%H:%M:%S %A, %d %B %Y %Z";
    if (strftime(sbuf, SBUF_SIZE, ofmt, &tm) == 0)
        fatal("strftime returned 0");
    printf("strftime() returned: %s\n", sbuf);

    exit(EXIT_SUCCESS);
}
