/* errno.c */
#include <errno.h>
#undef errno

__thread int errno = 0;
