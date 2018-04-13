/* isupper.c */
#include <ctype.h>

/*
 * isupper tests for an uppercase character.
 */
int (isupper)(int c)
{
    return (_Ctype[c] & _UP);
}
