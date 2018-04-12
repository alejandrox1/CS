/* isalnum.c */
#include <ctype.h>

/*
 * isalnum tests for an alphanumeric character.
 */
int (isalnum)(int c)
{
    return (_Ctype[c] & (_DI|_LO|_UP|_XA));
}
