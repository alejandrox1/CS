/* xctype.c _Ctype conversion table -- ASCII version. */
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#if EOF != -1 || UCHAR_MAX != 255
#error WRONG CTYPE TABLE
#endif


#define XDI (_DI|_XD) /* 0-9, 0-9, A-Z, a-z */
#define XLO (_LO|_XD) /* a-z, 0-9, A-Z, a-z */
#define XUP (_UP|_XD) /* A-Z, 0-9, A-Z, a-z */

/* Static data. */
static const short ctyp_tab[257] = {0, /* EOF */
      0, _BB, _BB, _BB, _BB, _BB, _BB, _BB,
    _BB, _CN, _CN, _CN, _CN, _CN, _BB, _BB,
    _BB, _BB, _BB, _BB, _BB, _BB, _BB, _BB,
    _BB, _BB, _BB, _BB, _BB, _BB, _BB, _BB,
    _SP, _PU, _PU, _PU, _PU, _PU, _PU, _PU,
    _PU, _PU, _PU, _PU, _PU, _PU, _PU, _PU,
    XDI, XDI, XDI, XDI, XDI, XDI, XDI, XDI,
    XDI, XDI, _PU, _PU, _PU, _PU, _PU, _PU,
    _PU, XUP, XUP, XUP, XUP, XUP, XUP, _UP,
    _UP, _UP, _UP, _UP, _UP, _UP, _UP, _UP,
    _UP, _UP, _UP, _UP, _UP, _UP, _UP, _UP,
    _UP, _UP, _UP, _PU, _PU, _PU, _PU, _PU,
    _PU, XLO, XLO, XLO, XLO, XLO, XLO, _LO,
    _LO, _LO, _LO, _LO, _LO, _LO, _LO, _LO,
    _LO, _LO, _LO, _LO, _LO, _LO, _LO, _LO,
    _LO, _LO, _LO, _PU, _PU, _PU, _PU, _BB,
};  /* Rest all match nothing */

const short *_Ctype = &ctyp_tab[1];
