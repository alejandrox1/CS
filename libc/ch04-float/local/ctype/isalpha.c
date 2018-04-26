/* isalpha.c */
#include <ctype.h>

/*
 * isalpha tests for an alphabetic character.
 */
int (isalpha)(int c)
{
    return (_Ctype[c] & (_LO|_UP|_XA));
}
