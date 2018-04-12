/* isspace.c */
#include <ctype.h>

/*
 * isspace tests for a spacing character.
 */
int (isspace)(int c)
{
    return (_Ctype[c] & (_CN|_SP|_XS));
}
