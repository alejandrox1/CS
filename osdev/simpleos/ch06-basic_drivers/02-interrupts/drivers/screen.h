#ifndef SCREEN_H
#define SCREEN_H


#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80

// Attribute byte for our default colour scheme.
#define WHITE_ON_BLACK 0X0f
#define RED_ON_WHITE 0xf4

// Screen cursor position IO ports
#define REG_SCREEN_CTRL 0X3D4
#define REG_SCREEN_DATA 0X3D5


void clear_screen();
void print_at(char *message, int col, int row);
void print(char *message);

#endif
