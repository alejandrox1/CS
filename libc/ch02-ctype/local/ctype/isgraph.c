/* isgraph.c */
#include <ctype.h>

/*
 * isgraphs tests for a graphic character.
 */
int (isgraph)(int c)
{
    return (_Ctype[c] & (_DI|_LO|_PUi|_UP|_XA));
}
