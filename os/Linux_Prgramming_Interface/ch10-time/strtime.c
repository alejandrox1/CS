/*
 * strtime.c
 *
 * Demonstrate the use of strftime() and strptime().
 *
 * Call strptime() using a given format to process the date+time input.
 * The conversion is then reversed by calling strftime(), which is supplied
 * with an output format, or a default if none is provided.
 *
 * Example:
 *      ./strtime "9:39:46pm 1 Feb 2011" "%I:%M:%S%p %d %b %Y"
 *
 *      ./strtime "9:39:46pm 1 Feb 2011" "%I:%M:%S%p %d %b %Y" "%F %T"
 *
 * Taken from The Linux Programming Interface.
 */
