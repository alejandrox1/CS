/* ctype.h */
#ifndef _CTYPE
#define _CTYPE

            /* _Ctype code bits. */
#define _XA  0X200 /* Extra alphabetic                     */
#define _XS  0x100 /* Extra space                          */
#define _BB  0x80  /* BEL(07), BS(08), etc.                */
#define _CN  0x40  /* CR(0D), FF(0C), HT(09), NL(), VT(0B) */
#define _DI  0x20  /* '0'-'9'                              */
#define _LO  0x10  /* 'a'-'z'                              */
#define _PU  0x08  /* Punctuation                          */
#define _SP  0x04  /* Space                                */
#define _UP  0x02  /* 'A'-'Z'                              */
#define _XD  0x01  /* '0'-'9', 'A'-'F', 'a'-'z'            */

            /* Declarations. */
int isalnum(int), isalpha(int), iscntrl(int), isdigit(int);
int isgraph(int), islower(int), isprint(int), ispunct(int);
int isspace(int), isupper(int), isxdigit(int);
int tolower(int), toupper(int);
extern const short *_Ctype, *_Tolower, *_Toupper;

            /* Macro overrides. */
#define isalnum(c)  (_Ctype[(int)(c)] & (_DI|_LO|_UP|_XA))         /* 0-9, a-z, A-Z, Extra               */
#define isalpha(c)  (_Ctype[(int)(c)] & (_LO|_UP|_XA))             /* a-z, A-Z, Extra                    */
#define iscntrl(c)  (_Ctype[(int)(c)] & (_BB|_CN))                 /* Control                            */
#define isdigit(c)  (_Ctype[(int)(c)] & _DI)                       /* 0-9                                */
#define isgraph(c)  (_Ctype[(int)(c)] & (_DI|_LO|_PU|_UP|_XA))     /* 0-9, a-z, punct, A-Z, Extra        */
#define islower(c)  (_Ctype[(int)(c)] & _LO)                       /* 0-9                                */
#define isprint(c)  (_Ctype[(int)(c)] & (_DI|_LO|_PU|_SP|_UP|_XA)) /* 0-9, a-z, punct, space, A-Z, Extra */
#define ispunct(c)  (_Ctype[(int)(c)] & _PU)                       /* Punctuation                        */
#define isspace(c)  (_Ctype[(int)(c)] & (_CN|_SP|_XS))             /* CN, space, Extra space             */
#define isupper(c)  (_Ctype[(int)(c)] & _UP)                       /* A-Z                                */
#define isxdigit(c) (_Ctype[(int)(c)] & _XD)                       /* 0-9, A-F, a-z                      */
#define tolower(c)  _Tolower[(int)(c)]
#define toupper(c)  _Toupper[(int)(c)]

#endif
