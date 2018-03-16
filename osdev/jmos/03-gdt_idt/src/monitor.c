#include "monitor.h"

// VGA framebuffer starts at 0xB8000
uint16_t *video_memory = (uint16_t *)0xB8000;
// Cursor position.
uint8_t cursor_x = 0;
uint8_t cursor_y = 0;


// move_cursor updates the hardware cursor.
static void move_cursor();
// scroll scrolls the text on screen up by one line.
static void scroll();

/******************************************************************************
 *                              Public API                                    *
 *****************************************************************************/
/*
 * monitor_put writes a character to screen.
 */
void monitor_put(char c)
{
	// Background color black(0), foreground color white(15).
	uint8_t backColor = 0;
	uint8_t foreColor = 15;
	uint8_t attrByte = (backColor << 4) | (foreColor &0x0F);
	uint16_t attribute = attrByte << 8;
	uint16_t *location;

	// Handle a backspace(0x08) by moving the cursor back.
	if (c == 0x08 && cursor_x)
	{
		cursor_x--;
	}
	// Handle a tab by increasing x to a point where it is divisible by 4.
	else if (c == '\t')
	{
		cursor_x = (cursor_x + 4) & ~(4-1);
		//cursor_x = cursor_x + (4 - cursor_x % 4);
	}
	// Handle carriage return.
	else if (c == '\r')
	{
		cursor_x = 0;
	}
	// Handle newline by moving cursor back to the left and incr row.
	else if (c == '\n')
	{
		cursor_x = 0;
		cursor_y++;
	}
	// Handle any other char.
	else if (c >= ' ')
	{
		location = video_memory + (cursor_y * 80 + cursor_x);
		*location = c | attribute;
		cursor_x++;
	}
	
	// Check if we are beyond end of screen.
	if (cursor_x >= 80)
	{
		cursor_x = 0;
		cursor_y++;
	}

	// Scroll if needed.
	scroll();
	// Move the hardware cursor.
	move_cursor();
}


/*
 * monitor_clear clears the screen.
 */
void monitor_clear()
{
	uint8_t attrByte = (0 /* black */ << 4) | (15 /* white */ & 0x0F);
	uint16_t blank = 0x20 /* space */ | (attrByte << 8);
	
	int i = 0;
	for (i=0; i<25*80; i++)
		video_memory[i] = blank;

	// Move hardware cursor back to origin.
	cursor_x = 0;
	cursor_y = 0;
	move_cursor();
}


/*
 * monitor_write writes a string to screen.
 */
void monitor_write(char *c)
{
	int i = 0;
	while (c[i])
		monitor_put(c[i++]);
}


/*
 * monitor_write_dec writes a number, base 10, to screen.
 */
void monitor_write_dec(uint32_t n)
{
	if (n == 0)
	{
		monitor_put('0');
		return;
	}

	uint32_t acc = n;
	char c[32];
	int i = 0;
	while (acc > 0)
	{
		c[i++] = '0' + acc % 10;
		acc /= 10;
	}
	c[i] = 0;

	char c2[32];
	int j = 0;
	c2[i--] = 0;
	while (i >= 0)
		c2[i--] = c[j++];

	monitor_write(c2);
}

/*
 * monitor_write_hex write a number, base 16, to screen.
 */
void monitor_write_hex(uint32_t n)
{
	int tmp;
	char noZeros = 1;

	monitor_write("0x");

	int i;
	for (i = 28; i >= 0; i -= 4)
	{
		tmp = (n >> i) & 0xF;
		if (tmp == 0 && noZeros !=0 && i != 0)
			continue;

		noZeros = 0;
		if (tmp >= 0xA)
			monitor_put(tmp - 0xA + 'A');
		else
		{
			monitor_put(tmp + '0');
		}
	}
}

/******************************************************************************
 *                              Private API                                   *
 *****************************************************************************/
/*
 * move_cursor updates the hardware cursor.
 */
static void move_cursor()
{
	uint16_t cursorLocation = cursor_y * 80 + cursor_x;
	outb(0x3D4, 14);                  // Tell VGA we are setting the high cursor byte.
	outb(0x3D5, cursorLocation >> 8); // Send high cursor byte.
	outb(0x3D4, 15);                  // Tell VGA we are setting the low cursor byte.
	outb(0x3D5, cursorLocation);      // Send low cursor byte.
}

/*
 * scroll scrolls the text on screen up by one line.
 */
static void scroll()
{
	// Get a space character with the default color attributes.
	uint8_t attributeByte = (0 /* black */ << 4) | (15 /* white */ & 0x0F);
	uint16_t blank = 0x20 /* space */ | (attributeByte << 8);

	// Row 25 is the end, so scroll up!
	if (cursor_y > 25)
	{
		// Move the current text chunk back in the buffer by a line.
		int i;
		for (i = 0*80; i < 24*80; i++)
			video_memory[i] = video_memory[i+80];

		// The last chunk should be blank.
		for (i = 24*80; i < 25*80; i++)
			video_memory[i] = blank;

		// Move cursor to last line.
		cursor_y = 24;
	}
}
