#include "vga.h"
#include <kernel/tty.h>
#include <stddef.h>
#include <string.h>

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t *const VGA_MEMEORY = (uint16_t *)0xB8000;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t *terminal_buffer;

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg);
static inline uint16_t vga_entry(unsigned char uc, uint8_t color);
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
void terminal_putchar(char c);
void terminal_write(const char *data, size_t size);
/******************************************************************************
 *                                Public API                                  *
 *****************************************************************************/
/*
 * terminal_initialize - Initialize VGA terminal global variables and clear
 * terminal.
 */
void terminal_initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = VGA_MEMORY;

    size_t x, y;
    for (y = 0; y < VGA_HEIGHT; ++y) {
        for (x = 0; x < VGA_WIDTH; ++x) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

/*
 * terminal_setcolor - Accessor for terminal_color global variable.
 * @color: uint8_t type sets terminal_color.
 */
void terminal_setcolor(uint8_t color) { terminal_color = color; }

/*
 * terminal_putchar - Write a character to the next position available on the
 * terminal.
 * @c: (char) character to be written.
 */
void terminal_putchar(char c) {
    unsigned char uc = c;
    terminal_putentryat(uc, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT)
            terminal_row = 0;
    }
}

/*
 * terminal_write - Write size number of characters from a string to the VGA
 * terminal.
 * @data: (const char *) String to be written.
 * @size: (size_t) Number of characters to be written.
 */
void terminal_write(const char *data, size_t size) {
    size_t i;
    for (i = 0; i < size; ++i)
        terminal_putchar(data[i]);
}

/*
 * terminal_writestring - Write a string to VGA terminal.
 * @data: (const char *) String to be written.
 */
void terminal_writestring(const char *data) {
    terminal_write(data, strlen(data));
}
/******************************************************************************
 *                               Private API                                  *
 *****************************************************************************/
/*
 * vga_entry_color - Return an attribute byte to set the background and
 * foreground for the vga terminal.
 * @fg: foreground color.
 * @bg: background color.
 */
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return (bg << 4) | fg;
}

/*
 * vga_entry - Return the proper format word to write to VGA terminal.
 * @uc: unsigned char to be writen to vga terminal.
 * @color: character and background color (see vga_entry_color).
 */
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t)uc | ((uint16_t)color << 8);
}

/*
 * terminal_putentryat - Write character of a given color to VGA terminal. Uses
 * vga_entry.
 * @c: (char) character to write to VGA terminal.
 * @color: (uint8_t) color scheme.
 * @x: (size_t) VGA position.
 * @y: (size_t) VGA position.
 */
void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}
