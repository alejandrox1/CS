#include <stdio.h>
#include "string.h"

int main()
{
    char string[5] = "abcd";
    printf("%s: %lu\n", string, strlen(string));

    return 0;
}
