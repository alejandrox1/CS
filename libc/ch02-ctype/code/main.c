#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

static void field_abort(int sig)
{
    puts("SUCCESS testing.");
    exit(EXIT_SUCCESS);
}

int main()
{
    char c = '0';
    if (isdigit(c))
        puts("It is a digit! :-)");

    assert(signal(SIGABRT, &field_abort) != SIG_ERR);
    int idx = 0;
    assert(idx);

    return 0;
}
