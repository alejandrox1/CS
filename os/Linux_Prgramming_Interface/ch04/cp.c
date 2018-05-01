/*
 * cp.c
 *
 * copy the file named argv[1] to a new file named argv[2].
 *
 * Taken from the Linux Programming Interface.
 */
#include <sys/stat.h>
#include <stdarg.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error_functions.h"

#ifndef BUFF_SIZE
#define BUFF_SIZE 1024
#endif


int main(int argc, char *argv[])
{
    int inputFd; //, outputFd, openFlags;
    //mode_t filePerms;
    //ssize_t numRead;
    //char buf[BUFF_SIZE];

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageError("%s old-file new-file\n", argv[0]);

    /* Open input and output files. */
    inputFd = open(argv[1], O_RDONLY);
    if (inputFd == -1)
        errExit("opening file %s", argv[1]);

    return 0;
}
