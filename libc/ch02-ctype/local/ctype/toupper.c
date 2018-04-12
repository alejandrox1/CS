/* toupper.c */
#include <ctype.h>

/*
 * toupper converts a character to uppercase.
 */
int (toupper)(int c)
{
    return (_Toupper[c]);
}
