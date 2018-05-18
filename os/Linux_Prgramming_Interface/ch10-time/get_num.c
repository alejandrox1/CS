/*
 * get_num.c
 *
 * Functions to process numerica command-line arguments.
 *
 * Taken from the Linux Programming Interface.
 */
#include "get_num.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

static void
gnFail(const char* fname, const char* msg, const char* arg, const char* name);
static long
getNum(const char* fname, const char* arg, int flags, const char* name);
/******************************************************************************
 *                             Public API                                     *
 *****************************************************************************/
/**
 * getInt - convert a numeric command line argument string to an integer.
 * See getNum() for a description of the arguments to this function.
 */
int getInt(const char* arg, int flags, const char* name)
{
    long res;

    res = getNum("getInt", arg, flags, name);

    if (res > INT_MAX || res < INT_MIN)
        gnFail("getInt", "integer out of range", arg, name);

    return (int)res;
}

/**
 * getLong - Convert a numeric command-line argument into a long integer.
 * See getNUm() for a description of the arguments to this function.
 */
long getLong(const char* arg, int flags, const char* name)
{
    return getNum("getLong", arg, flags, name);
}

/******************************************************************************
 *                             Private API                                    *
 *****************************************************************************/
/**
 * gnFail - Print a diagnostic message that contains a function name, value of
 * a command-line argument, the name of that command-line argument, and a
 * disagnostic message.
 *
 * @fname: Function name.
 * @msg: Diagnostic message.
 * @arg: Command-line argument.
 * @name: Name of command-line argument.
 */
static void
gnFail(const char* fname, const char* msg, const char* arg, const char* name)
{
    fprintf(stderr, "%s error", fname);
    if (name != NULL)
        fprintf(stderr, " (in %s)", name);
    fprintf(stderr, ": %s\n", msg);
    if (arg != NULL && *arg != '\0')
        fprintf(stderr, "\toffending text: %s\n", arg);

    exit(EXIT_FAILURE);
}

/**
 * getNum - Convert a numeric command-line argument ('arg') into a long integer
 * and return it.
 * 'fname' and 'name' are used to print a diagnostic message in case an error
 * is detected when processing 'arg'.
 *
 * @fname: Name of caller.
 * @arg: Command-line argument.
 * @flags: Bit mask of flags controlling how the conversion is done and what
 * diagnostic checks are performed on the numeric result.
 * @name: Name associated with command line argument 'arg'.
 */
static long
getNum(const char* fname, const char* arg, int flags, const char* name)
{
    long  res;
    char* endptr;
    int   base;

    if (arg == NULL || *arg == '\0')
        gnFail(fname, "null or empty string", arg, name);

    base = (flags & GN_ANY_BASE)
               ? 0
               : (flags & GN_BASE_8) ? 8 : (flags & GN_BASE_16) ? 16 : 10;

    errno = 0;
    res   = strtol(arg, &endptr, base);
    if (errno != 0)
        gnFail(fname, "strtol() failed", arg, name);

    if (*endptr != '\0')
        gnFail(fname, "nonumeric characters", arg, name);

    if ((flags & GN_NONNEG) && res < 0)
        gnFail(fname, "negative value not allowed", arg, name);

    if ((flags & GN_GT_0) && res <= 0)
        gnFail(fname, "value must be > 0", arg, name);

    return res;
}
