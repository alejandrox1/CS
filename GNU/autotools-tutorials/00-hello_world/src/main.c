#include <config.h>
#include <stdio.h>

int main(void)
{
    //char PACKAGE_STRING[] = "amhello-1.0";
    puts("Hello world!");
    printf("This is package %s\n", PACKAGE_STRING);
    return 0;
}
