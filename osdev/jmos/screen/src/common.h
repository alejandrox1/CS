/*
 * common.h 
 *
 * From JamesM' kernel development tutorials.
 */
#ifndef COMMON_H
#define COMMON_H

/*
 * typedefs to standardize sizes across platforms.
 * Writfen for 32-bit x86.
 */
typedef unsigned int   u32int_t;
typedef          int   s32int_t;
typedef unsigned short u16int_t;
typedef          short s16int_t;
typedef unsigned char  u8int_t;
typedef          char  s8int_t;


// outb writes a byte to port.
void outb(u16int_t port, u8int_t value);
// inb reads a byte from port.
u8int_t inb(u16int_t port);
// inw reads a word from port.
u16int_t inw(u16int_t port);


#endif
