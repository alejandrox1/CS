//#include <config.h>
#include <stdio.h>

int main(void)
{
    char PACKAGE_STRING[] = "hello-v0.0.0";
    puts("Hello world!");
    printf("This is package %s\n", PACKAGE_STRING);
    return 0;
}
