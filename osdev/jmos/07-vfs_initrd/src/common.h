/*
 * common.h 
 *
 * From JamesM' kernel development tutorials.
 */
#ifndef COMMON_H
#define COMMON_H

#include "types.h"


/*
 * Driver funcs.
 */
// outb writes a byte to port.
void outb(uint16_t port, uint8_t value);

// inb reads a byte from port.
uint8_t inb(uint16_t port);

// inw reads a word from port.
uint16_t inw(uint16_t port);


/*
 * Memory
 */
// memset writes len copies of val into dest.
void memset(uint8_t *dest, uint8_t val, uint32_t len);

// memcpy copies len bytes from src to dest.
void memcpy(uint8_t *dest, const uint8_t *src, uint32_t len);


/*
 * Strings
 */
// strlen
int strlen(char *src);

// strcmp
int strcmp(const char *str1, const char *str2);

// strcpy copies a NULL_termindated string from src into dest, and returns
// dest.
char *strcpy(char *dest, const char *src);

#endif
