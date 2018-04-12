/* Test assert macro. */
#define NDEBUG
#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

static int val = 0;

/*
 * field_abort handles a SIGABRT signal.
 * For htis test, we are assuming that a failed assert (active form - no
 * NDEBUG) will occur when we test the global val to be anything but equal to
 * 1.
 */
static void field_abort(int sig)
{
    if (val == 1)
    {
        puts("SUCCESS testing <assert.h>");
        exit(EXIT_SUCCESS);
    }
    else
    {
        puts("FAILURE testing <assert.h>");
        exit(EXIT_FAILURE);
    }
}

/*
 * dumy asumes that assert was included with NDEBUG defined so a failed assert
 * will return void.
 */
static void dummy()
{
    int i = 0;

    assert(i == 0);
    assert(i == 1);
}

#undef NDEBUG
#include <assert.h>

int main()
{
    assert(signal(SIGABRT, &field_abort) != SIG_ERR);
    
    dummy();
    
    assert(val == 0);
    ++val;
    
    fputs("Sample assertion failure message --\n", stderr);
    assert(val == 0);

    /*
     * If program terminates here then there was a problem with the signal
     * handler.
     */
    puts("FAILURE testing <assert.h>");
    exit(EXIT_FAILURE);
}
