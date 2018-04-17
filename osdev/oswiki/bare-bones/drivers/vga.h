#ifndef _VGA_H
#define _VGA_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__linux__)
#error "Cross-compiler is not being used"
#endif

#if !defined(__i386__)
#error "This needs to be compiled for a 32-bit ix86 target"
#endif



#endif
