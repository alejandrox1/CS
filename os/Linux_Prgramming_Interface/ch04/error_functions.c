/*
 * error_functions.c
 *
 *
 * Taken from the Linux Programming Interface.
 */
#include <stdarg.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "error_functions.h"

#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

typedef enum { FALSE, TRUE } Boolean;

static void outputError(Boolean useErr, int err, Boolean flushStdout, const char* format, va_list ap);
NORETURN static void terminate(Boolean useExit3);
/******************************************************************************
 *                            Public API                                      *
 *****************************************************************************/
/**
 * errExit - Displays an error messgae including an errno diagnostic, and
 * terminates the process.
 */
void errExit(const char* format, ...)
{
    va_list argList;

    va_start(argList, format);
    outputError(TRUE, errno, TRUE, format, argList);
    va_end(argList);

    terminate(TRUE);
}

/**
 * fatal - Print an error message.
 */
void fatal(const char* format, ...)
{
    va_list argList;

    va_start(argList, format);
    outputError(FALSE, 0, TRUE, format, argList);
    va_end(argList);

    terminate(TRUE);
}

/**
 * usgaeError - Prints a command usage error message and terminates the
 * process.
 */
void usageError(const char* format, ...)
{
    va_list argList;

    fflush(stdout);

    fprintf(stderr, "Usage:");
    va_start(argList, format);
    vfprintf(stderr, format, argList);
    va_end(argList);

    fflush(stderr);
    exit(EXIT_FAILURE);
}

/******************************************************************************
 *                            Private API                                     *
 *****************************************************************************/
/**
 * outputError - Diagnoses errno by:
 *  - outputting a string containing the error number along with the
 *    corresponding message from strerror().
 *
 *  - outputing the caller-supplied error message specified in 'format' and
 *    'ap'.
 */
static void outputError(Boolean useErr, int err, Boolean flushStdout, const char* format, va_list ap)
{
#define BUFF_SIZE 500
    char buf[BUFF_SIZE], userMsg[BUFF_SIZE], errText[BUFF_SIZE];

    vsnprintf(userMsg, BUFF_SIZE, format, ap);

    if (useErr)
        snprintf(errText, BUFF_SIZE, " [%d %s]", err, strerror(err));
    else
        snprintf(errText, BUFF_SIZE, ":");

    snprintf(buf, BUFF_SIZE, "ERROR:%s %s\n", errText, userMsg);

    if (flushStdout)
        fflush(stdout);
    fputs(buf, stderr);
    fflush(stderr);
}


/**
 * terminate - Terminates calling process.
 */
static void terminate(Boolean useExit3)
{
    char *s;

    s = getenv("EF_DUMPCORE");

    if (s != NULL && *s != '\0')
        abort();
    else if (useExit3)
        exit(EXIT_FAILURE);
    else
        _exit(EXIT_FAILURE);
}
