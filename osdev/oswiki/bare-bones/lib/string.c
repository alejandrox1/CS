#include <string.h>
#include <stddef.h>

/*
 * strlen - Find the length of a string.
 * @s: The string o be sized.
 */
size_t strlen(const char *s)
{
    const char *sc;

    for (sc = s; *sc != '\0'; ++sc)
        /* Nothing. */;
    return sc - s;
}
