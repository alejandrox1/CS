/* isdigt.c */
#include <ctype.h>

/*
 * isdigit tests for a digit.
 */
int (isdigit)(int c)
{
    return (_Ctype[c] & _DI);
}
