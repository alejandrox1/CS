#include "monitor.h"

// VGA framebuffer starts at 0xB8000
u16int_t *video_memory = (u16int_t *)0xB8000;
// Cursor position.
u8int_t cursor_x = 0;
u8int_t cursor_y = 0;


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
	u8int_t backColor = 0;
	u8int_t foreColor = 15;
	u8int_t attrByte = (backColor << 4) | (foreColor &0x0F);
	u16int_t attribute = attrByte << 8;
	u16int_t *location;

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
	u8int_t attrByte = (0 /* black */ << 4) | (15 /* white */ & 0x0F);
	u16int_t blank = 0x20 /* space */ | (attrByte << 8);
	
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



/******************************************************************************
 *                              Private API                                   *
 *****************************************************************************/
/*
 * move_cursor updates the hardware cursor.
 */
static void move_cursor()
{
	u16int_t cursorLocation = cursor_y * 80 + cursor_x;
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
	u8int_t attributeByte = (0 /* black */ << 4) | (15 /* white */ & 0x0F);
	u16int_t blank = 0x20 /* space */ | (attributeByte << 8);

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
