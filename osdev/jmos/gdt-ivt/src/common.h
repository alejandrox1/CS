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
typedef unsigned int   uint32_t;
typedef          int   int32_t;
typedef unsigned short uint16_t;
typedef          short int16_t;
typedef unsigned char  uint8_t;
typedef          char  int8_t;


// outb writes a byte to port.
void outb(uint16_t port, uint8_t value);
// inb reads a byte from port.
uint8_t inb(uint16_t port);
// inw reads a word from port.
uint16_t inw(uint16_t port);


#endif
