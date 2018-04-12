/* tolower.c */
#include <ctype.h>

/*
 * tolower converts a character to lowercase.
 */
int (tolower)(int c)
{
    return (_Tolower[c]);
}
