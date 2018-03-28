/*
 * common.c
 *
 * From JamesM' kernel development tutorial.
 */
#include "types.h"
#include "common.h"

/******************************************************************************
 *                                Port IO                                     *
 *****************************************************************************/
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

/******************************************************************************
 *                                  Memory                                    *
 *****************************************************************************/
/*
 * memset writes len copies of val into dest.
 */
void memset(uint8_t *dest, uint8_t val, uint32_t len)
{
	for ( ; len != 0; len--)
		*dest++ = val;
}

/*
 * memcpy copies len bytes from src to dest.
 */
void memcpy(uint8_t *dest, const uint8_t *src, uint32_t len)
{
    const uint8_t *sp = (const uint8_t *)src;
    uint8_t       *dp = (uint8_t *)dest;
    for ( ; len != 0; len--)
        *dp++ = *sp++;
}

/******************************************************************************
 *                                   Strings                                  *
 *****************************************************************************/
/*
 * strcpy copies a NULL-terminated string from src into dest, and returns dest.
 */
char *strcpy(char *dest, const char *src)
{
    char *temp = dest;
    while ((*dest++ = *src++));
    return temp;
}

/*
 * strlen
 */
int strlen(char *src)
{
    int i = 0;
    while (*src++)
        i++;
    return i;
}

/*
 * strcmp compares two strings.
 * If str1 == str2, return 0.
 * If str1 < str2, return -1.
 * If str1 > str2, return 1.
 */
int strcmp(const char *str1, const char *str2)
{
    while (str1 && (*str1 == *str2))
    {
        str1++;
        str2++;
    }
    int res = *(const unsigned char *)str1 - *(const unsigned char *)str2;
    if (res < 0)
        return -1;
    else if (res > 0)
        return 1;
    return 0;
}
