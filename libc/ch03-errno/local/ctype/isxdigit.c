/* isxdigit.c */
#include <ctype.h>

/*
 * isxdigit tests for a hexadecimal digit.
 */
int (isxdigit)(int c)
{
    return (_Ctype[c] & _XD);
}
