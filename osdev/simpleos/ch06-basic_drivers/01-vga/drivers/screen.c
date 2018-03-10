#include "screen.h"
#include "ports.h"
#include "../kernel/util.h"


/*
 * Private functions.
 */

int get_screen_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);
int get_cursor_offset();
void set_cursor_offset(int offset);
int handle_scrolling(int offset);
int print_char(char character, int col, int row, char attribute_byte);


/************************************
 * 		Publick Kernel API			*
 ************************************/
/*
 * clear_screen
 */
void clear_screen()
{
	int screen_size = MAX_ROWS * MAX_COLS;
	char *screen = (char *)VIDEO_ADDRESS;

	int i;
	for (i=0; i<screen_size; i++)
	{
		screen[i*2] = ' ';
		screen[i*2+1] = WHITE_ON_BLACK;
	}
	set_cursor_offset(get_screen_offset(0,0));
}

/*
 * print_at prints a message at a specified location.
 * If col and row are negative, use current offset.
 */
void print_at(char *message, int col, int row)
{
	// Set cursor
	int offset;
	if (row >= 0 && col >= 0)
		offset = get_screen_offset(col, row);
	else
	{
		offset = get_cursor_offset();
		row = get_offset_row(offset);
		col = get_offset_col(offset);
	}

	// Loop through message and print it.
	int i=0;
	while (message[i] != '\0')
	{
		offset = print_char(message[i++], col, row, WHITE_ON_BLACK);
		row = get_offset_row(offset);
		col = get_offset_col(offset);
	}
}

/*
 * print
 */
void print(char *message)
{
	print_at(message, -1, -1);
}


/************************************
 * 		Private Kernel methods 		*
 ************************************/
/*                                                                              
 * print_char prints a char on screen given a column and a row or a cursor      
 * position directly accessing the video memory.                                
 *                                                                              
 * If row and col are negative we will print at the current cursor position.    
 * If attribute_byte is not set we will default to white on black.              
 * Returns the newest offset for the next character to be printed.              
 */                                                                             
int print_char(char character, int col, int row, char attribute_byte)           
{                                                                               
	// Pointer to start of video memory.                                        
	unsigned char *vidmem = (unsigned char *)VIDEO_ADDRESS;                     

	// Set to default.   
	if (!attribute_byte)   
		attribute_byte = WHITE_ON_BLACK;                                         
	
	// Print a red 'E' if row and col are notright.   
	if (row >= MAX_ROWS || col >= MAX_COLS)   
	{   
		vidmem[2*(MAX_ROWS)*(MAX_COLS)-2] = 'E';      
		vidmem[2*(MAX_ROWS)*(MAX_COLS)-1] = RED_ON_WHITE;     
		return get_screen_offset(col, row);
	} 
	
	// Get video memory offset for screen location.    
	int offset;         
	if (col >= 0 && row >=0)
		offset = get_screen_offset(col, row);      
	else 
		offset = get_cursor_offset();
	
	// If newline, set offset to the end of current row, so it will be advanced 
	// to the first col of the next row.      
	if (character == '\n')    
	{ 
		row = get_offset_row(offset);      
		offset = get_screen_offset(0, row+1);  
	}  
	// Otherwise, write char and attribute to video memory at offset.     
	else   
	{
		vidmem[offset] = character;  
		vidmem[offset+1] = attribute_byte;  
		offset += 2;
	}  
	
	// Make scrolling adjustment.   
	offset = handle_scrolling(offset);     
	// Update cursor position.  
	set_cursor_offset(offset);    
	return offset;                                                              
} 


/*
 * get_screen_offset  
 */
int get_screen_offset(int col, int row)
{
	return 2 * (row * MAX_COLS + col);
}

/*
 * get_offset_row
 */
int get_offset_row(int offset)                                                  
{                                                                               
	    return offset / (2 * MAX_COLS);                                              
}

/*
 *
 */
int get_offset_col(int offset)
{
	return (offset - (get_offset_row(offset) * MAX_COLS * 2)) / 2;
}

/*
 * get_cursor_offset use VGA ports to get the current cursor position.
 * 1. Ask for high byte of the cursor offset (data 14)
 * 2. Ask for low byte (data 15)
 */
int get_cursor_offset()
{	
	port_byte_out(REG_SCREEN_CTRL, 14);
	int offset = port_byte_in(REG_SCREEN_DATA) << 8;
	port_byte_out(REG_SCREEN_CTRL, 15);
	offset += port_byte_in(REG_SCREEN_DATA);
	return offset * 2;
}

/*
 * set_cursor_offset
 */
void set_cursor_offset(int offset)
{
	offset /= 2;
	port_byte_out(REG_SCREEN_CTRL, 14);
	port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
	port_byte_out(REG_SCREEN_CTRL, 15);
	port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

/*
 * handle_scrolling
 */
int handle_scrolling(int offset)
{
	// Check if offset is over the screen size and scroll.                      
	if (offset >= MAX_ROWS * MAX_COLS * 2)                                      
	{          
		int i;    
		for (i=1; i<MAX_ROWS; i++) 
			memory_copy((char *)(get_screen_offset(0, i) + VIDEO_ADDRESS), 
					    (char *)(get_screen_offset(0, i-1) + VIDEO_ADDRESS), 
						MAX_COLS * 2);                                          

		// Blank line at bottom    
		char *last_line = (char *)(get_screen_offset(0, MAX_ROWS-1) + VIDEO_ADDRESS); 
		for (i=0; i<MAX_COLS*2; i++) 
			last_line[i] = 0;       
		offset -= 2 * MAX_COLS;                                                 
	}
	return offset;
}
