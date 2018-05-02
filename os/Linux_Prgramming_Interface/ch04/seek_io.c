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

