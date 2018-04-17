#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>

#ifndef __HAVE_ARCH_STRLEN
extern size_t strlen(const char *s);
#endif

#endif
