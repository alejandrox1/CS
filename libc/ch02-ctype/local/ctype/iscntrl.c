/* iscntrl.c */
#include <ctype.h>

/*
 * iscntrl tests for a control character.
 */
int (iscntrl)(int c)
{
    return (_Ctype[c] & (_BB|_CN));
}
