/*
 * monitor.h
 *
 * From JamesM's kernel development tutorial.
 */
#ifndef MONITOR_H
#define MONITOR_H

#include "common.h"


// monitor_put writes a character to screen.
void monitor_put(char c);

// monitor_clear clears the screen.
void monitor_clear();

// monitor_write writes a null-terminated ASCII string to screen.
void monitor_write(char *c);

// monitor_write_dec write a decimal number to screen.
void monitor_write_dec(uint32_t n);

// monitor_write_hex writes a hex number to screen.
void monitor_write_hex(uint32_t n);


#endif
