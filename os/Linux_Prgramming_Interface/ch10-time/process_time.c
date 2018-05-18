/*
 * process_time.c
 *
 * Demonstrate usage of clock(3) and times(2) to retrieve a process's virtual
 * time.
 *
 * Usage:
 *      
 *      pocess_time [num-calls]
 *
 * Make 'num-calls` to getpid(), then display process time to stdout.
 *
 * Taken from The Linux Programming INterface.
 */
#include <sys/times.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "get_num.h"
#include "error_functions.h"

static void displayProcessTimes(const char *msg);


int main(int argc, char *argv[])
{
    int numCalls, j;

    printf("CLOCKS_PER_SEC=%ld  sysconf(_SC_CLK_TCK)=%ld\n\n", 
            (long)CLOCKS_PER_SEC, sysconf(_SC_CLK_TCK));

    displayProcessTimes("At program start:\n");

    /* 
     * Call getppid() a large number of times, so that some user and system CPU
     * time are consumed 
     */
    numCalls = (argc > 1) ? getInt(argv[1], GN_GT_0, "num-calls") : 100000000;
    for (j = 0; j < numCalls; j++)
        (void) getppid();

    displayProcessTimes("After getppid() loop:\n");

    exit(EXIT_SUCCESS);
}


/**
 * displayProcessTimes - Display message and process times.
 *
 * @msg: message to be displayed.
 */
static void displayProcessTimes(const char* msg)
{
    struct tms t;
    clock_t clockTime;
    static long clockTicks = 0;

    if (msg != NULL)
        printf("%s", msg);

    /* Fetch clock ticks on first call. */
    if (clockTicks == 0)
    {
        clockTicks = sysconf(_SC_CLK_TCK);
        if (clockTicks == -1)
            errExit("sysconf");
    }

    clockTime = clock();
    if (clockTime == -1)
        errExit("clock");

    printf("\t\tclock() returns: %ld clocks-per-sec (%.2f secs)\n", 
            (long)clockTime, (double)clockTime / CLOCKS_PER_SEC);

    if (times(&t) == -1)
        errExit("times");
    printf("\t\ttimes() yields: user CPU=%.2f; system CPU: %.2f\n",
            (double)t.tms_utime / clockTicks,
            (double)t.tms_stime / clockTicks);
}
