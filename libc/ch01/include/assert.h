#undef assert    /* Remove existing definition. */

#ifdef NDEBUG
    /* Passive form. */
    #define assert(test) ((void)0)             
#else
    void _Assert(char *);
    /* Macros. */
    #define _STR(X) _VAL(X)
    #define _VAL(X) #X
    /* Active form. */
    #define assert(test) ((test) ? (void)0  \
            : _Assert(__FILE__ ":" _STR(__LINE__) " " #test))
#endif
