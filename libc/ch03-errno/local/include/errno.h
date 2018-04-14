/* errno.h */
#ifndef _ERRNO_H

/*
 * The includer defined __need_Emath if she wants only the definitions of EDOM
 * and ERANGE, and not everything else.
 */
#ifndef __need_Emath
# define _ERRNO_H
#endif

// Taken from Ubuntu GLIBC 2.23-0ubuntu10.
#include <bits/errno.h>
#undef __need_Emath

#ifndef errno
extern int errno;
#endif

#undef
