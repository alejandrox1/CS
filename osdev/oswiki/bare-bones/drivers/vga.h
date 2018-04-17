#ifndef _VGA_H
#define _VGA_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


void terminal_initialize(void);

void terminal_setcolor(uint8_t color);

void terminal_writestring(const char *data);


#endif
