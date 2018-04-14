/* test_errno.c */
#include <errno.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main()
{
    assert(errno == 0);
    perror("No error reported as");

    errno = ERANGE;
    assert(errno == ERANGE);
    perror("Range error reported as");

    errno = 0;
    assert(errno == 0);
    
    sqrt(-1.0);
    assert(errno == EDOM);
    perror("Domain error reported as");

    puts("Success testing <errno.h>");
    return (0);
}
