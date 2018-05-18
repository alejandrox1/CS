/* show_time.c
 *
 * A short program that allows us to see the effects of locale and timezone on
 * some of the functions that deal with time.
 *
 * Try running this program with command lines such as the following:
 *
 *      ./show_time
 *      TZ=":Pacific/Auckland" ./show_time
 *      TZ=":US/Central" ./show_time
 *      TZ=":CET" ./show_time
 */
#include "error_functions.h"
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BUF_SIZE 200

int main(void)
{
    time_t     t;
    struct tm* loc;
    char       buf[BUF_SIZE];

    if (setlocale(LC_ALL, "") == NULL)
        errExit("setlocale"); /* Use locale settings in conversions */

    t = time(NULL);

    printf("ctime() of time() value is:  %s", ctime(&t));

    loc = localtime(&t);
    if (loc == NULL)
        errExit("localtime");

    printf("asctime() of local time is:  %s", asctime(loc));

    if (strftime(buf, BUF_SIZE, "%A, %d %B %Y, %H:%M:%S %Z", loc) == 0)
        fatal("strftime returned 0");
    printf("strftime() of local time is: %s\n", buf);

    exit(EXIT_SUCCESS);
}
