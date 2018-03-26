/*
 * common.c
 *
 * From JamesM' kernel development tutorial.
 */
#include "common.h"

/* 
 * outb writes a byte out to the specified port.
 */
void outb(uint16_t port, uint8_t value)
{
	asm volatile("outb %1, %0" : : "dN" (port), "a" (value));
}

/*
 * inb reads a byte from the specified port.
 */
uint8_t inb(uint16_t port)
{
	uint8_t ret;
	asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

/*
 * inw reads a word (16 bits) from the specified port.
 */
uint16_t inw(uint16_t port)
{
	uint16_t ret;
	asm volatile("inw %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

/*
 * memset write len copies of val into dest.
 */
void memset(uint8_t *dest, uint8_t val, uint32_t len)
{
	for ( ; len != 0; len--)
		*dest++ = val;
}
