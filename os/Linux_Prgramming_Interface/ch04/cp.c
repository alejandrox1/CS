/*
 * cp.c
 *
 * copy the file named argv[1] to a new file named argv[2].
 *
 * Taken from the Linux Programming Interface.
 */
#include "error_functions.h"
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef BUFF_SIZE
#define BUFF_SIZE 1024
#endif

int main(int argc, char* argv[])
{
    int     inputFd, outputFd, openFlags;
    mode_t  filePerms;
    ssize_t numRead;
    char    buf[BUFF_SIZE];

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageError("%s old-file new-file\n", argv[0]);

    /* Open input and output files. */
    inputFd = open(argv[1], O_RDONLY);
    if (inputFd == -1)
        errExit("opening file %s", argv[1]);

    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH |
                S_IWOTH; /* rw-rw-rw- */
    outputFd = open(argv[2], openFlags, filePerms);
    if (outputFd == -1)
        errExit("opening file %s", argv[2]);

    /* Transfer data until we encounter end of input or an error. */
    while ((numRead = read(inputFd, buf, BUFF_SIZE)) > 0)
        if (write(outputFd, buf, numRead) != numRead)
            fatal("couldn't write whole buffer");

    if (numRead == -1)
        errExit("read");

    if (close(inputFd) == -1)
        errExit("close input");
    if (close(outputFd) == -1)
        errExit("close output");

    exit(EXIT_SUCCESS);
}
