/* islower.c */
#include <ctype.h>

/*
 * islower tests for a lowercase character.
 */
int (islower)(int c)
{
    return (_Ctype[c] & _LO);
}
