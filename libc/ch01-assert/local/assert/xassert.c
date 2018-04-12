/* _Assert function. */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * _Assert prints an assertion message and aborts.
 */
void _Assert(char *msg)
{ 
    fputs(msg, stderr);
    fputs(" -- assertion failed :-)\n", stderr);
    abort();
}
