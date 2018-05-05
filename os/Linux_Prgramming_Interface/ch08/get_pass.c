#include "get_pass.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include "error_functions.h"

char* get_pass(const char* prompt)
{
    struct termios oflags, nflags;
#define PW_LEN 64
    char *password = (char *)malloc(sizeof(char)*PW_LEN);

    /* Dissable echo. */
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
        errExit("tcsetattr");

    /* Get password from stdin. */
    printf("%s", prompt);
    fgets(password, sizeof(password), stdin);
    password[strlen(password)-1] = '\0';

    /* Restore terminal. */
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
        errExit("tcsetattr");

    return password;
}
