/*
 * error_functions.h
 *
 * Taken from the Linux Programming Interface.
 */
#ifndef ERROR_FUNCTIONS_H
#define ERROR_FUNCTIONS_H

#ifdef __GNUC__
#define NORETURN __attribute__ ((__noreturn__))
#else
#define NORETURN
#endif

void errExit(const char* format, ...) NORETURN;
void fatal(const char* format, ...) NORETURN;
void usageError(const char* format, ...) NORETURN;

#endif
