/*
 * common.c
 *
 * From JamesM' kernel development tutorial.
 */
#include "common.h"

/* 
 * outb writes a byte out to the specified port.
 */
void outb(u16int_t port, u8int_t value)
{
	asm volatile("outb %1, %0" : : "dN" (port), "a" (value));
}

/*
 * inb reads a byte from the specified port.
 */
u8int_t inb(u16int_t port)
{
	u8int_t ret;
	asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

/*
 * inw reads a word (16 bits) from the specified port.
 */
u16int_t inw(u16int_t port)
{
	u16int_t ret;
	asm volatile("inw %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}
