#ifndef _STRING_H
#define _STRING_H 1

#include <sys/cdefs.h>
#include <stddef.h>

#ifdef __cpluplus
extern "C" {
#endif

int memcmp(const void* aptr, const void* bptr, size_t size);
void *memcpy(void* __restrict dstptr, const void* __restrict srcptr, size_t size);
void *memmove(void* dstptr, const void* srcptr, size_t size);
void *memset(void* bufptr, int value, size_t size);
extern size_t strlen(const char *s);

#ifdef __cplusplus
}
#endif

#endif
