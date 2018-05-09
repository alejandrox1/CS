/*
 * curr_time.c
 *
 * Display the current time.
 *
 * Taken from The Linux Programming Interface.
 */
#include <stdio.h>
#include <time.h>
#include "curr_time.h"

#define BUF_SIZE 1000

/**
 * currTime - Return a string containing the current time formatted according
 * to the specification in 'format'.
 * If 'format' is NULL, we use "%c" as a specifier (which gives the date and
 * time as specified in ctime(3), but withouth the trailing newline).
 * Return null on error.
 * @format: String. See strftime(3) for specifiers.
 */
char* currTime(const char* format)
{
    static char[BUF_SIZE]; /* non-reentrant. */
    time_t t;
    size_t s;
    struct tm *tm;

    t = time(NULL);
    tm = localtime(&t);
    if (tm == NULL)
        return NULL;

    s = strftime(buf, BUF_SIZE, (format != NULL) ? format : "%c", tm);

    return (s == 0) ? NULL : buf;
}
