/*
 * seek_io.c
 *
 * Demonstrate theuse of lseek() and file IO system calls.
 *
 * Usage: seek_io file {r<lenght>|R<lenght>|w<string>|s<offset>}
 *
 * This programs opens the file named on its command line, and then performs
 * the file IO operations specified by the reamining command line arguments:
 *
 *      r<lenght>   Read 'lenght' bytes from the file at current file offset,
 *                  and display them as text.
 *
 *      R<lenght>   Read 'lenght' bytes from the file at current file offset,
 *                  and display them as hex.
 *
 *      w<string>   Write 'string' at current file offset.
 *
 *      s<offset>   Set the file offset to 'offset'.
 *
 * Example:
 *
 *      seek_io file wxyz s1 r2
 *
 * Taken from the Linux Programming INterface.
 */
#include "error_functions.h"
#include "get_num.h"
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    size_t  len;
    off_t   offset;
    int     fd, ap, j;
    char*   buf;
    ssize_t numRead, numWritten;

    if (argc < 3 || strcmp(argv[1], "--help") == 0)
        usageError("%s file {r<lenght>|R<lenght>|w<string>|s<offset>}...\n",
                   argv[0]);

    fd = open(argv[1],
              O_RDWR | O_CREAT,
              S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (fd == -1)
        errExit("open");

    for (ap = 2; ap < argc; ap++)
    {
        switch (argv[ap][0])
        {
        case 'r': /* Display bytes at current offset, as text. */
        case 'R': /* Display bytes at current offset, as hex.  */
            len = getLong(&argv[ap][1], GN_ANY_BASE, argv[ap]);

            buf = malloc(len);
            if (buf == NULL)
                errExit("malloc");

            numRead = read(fd, buf, len);
            if (numRead == -1)
                errExit("read");

            if (numRead == 0)
            {
                printf("%s: end-of-file\n", argv[ap]);
            }
            else
            {
                printf("%s: ", argv[ap]);
                for (j = 0; j < numRead; ++j)
                {
                    if (argv[ap][0] == 'r')
                        printf("%c",
                               isprint((unsigned char)buf[j]) ? buf[j] : '?');
                    else
                        printf("%02x ", (unsigned int)buf[j]);
                }
                printf("\n");
            }
            free(buf);
            break;

        case 'w': /* Write string at current offset. */
            numWritten = write(fd, &argv[ap][1], strlen(&argv[ap][1]));
            if (numWritten == -1)
                errExit("write");
            printf("%s: wrote %ld bytes\n", argv[ap], (long)numWritten);
            break;

        case 's': /* Change file offset. */
            offset = getLong(&argv[ap][1], GN_ANY_BASE, argv[ap]);
            if (lseek(fd, offset, SEEK_SET) == -1)
                errExit("lseek");
            printf("%s: seek succeeded\n", argv[ap]);
            break;

        default:
            cmdLineErr("Argument must start with [rRws]: %s\n", argv[ap]);
        }
    }

    if (close(fd) == -1)
        errExit("close");

    exit(EXIT_SUCCESS);
}
