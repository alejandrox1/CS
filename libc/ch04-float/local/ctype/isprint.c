/* isprint.c */
#include <ctype.h>

/*
 * isprint tests for a printable character.
 */
int (isprint)(int c)
{
    return (_Ctype[c] & (_DI|_LO|_PU|_SP|_UP|_XA));
}
