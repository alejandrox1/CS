#include <stdio.h>
#include <stdlib.h>

void abort(void) __attribute__((__noreturn__))
{
#if defined(__is_libk)
    // TODO: Add proper kernel panic.
    printf("kernel: panic abort()\n");
#else
    // TODO: Abnormally terminate the process s if by SIGABRT.
    printf("abort()\n");
#endif
    while (1){}
    __builtin_unreachable();
}
