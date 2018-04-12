/* ispunct.c */
#include <ctype.h>

/*
 * ispunct tests for a punctuation character.
 */
int (ispunct)(int c)
{
    return (_Ctype[c] & _PU);
}
