/*
 * error_functions.h
 *
 * Taken from the Linux Programming Interface.
 */
#ifndef ERROR_FUNCTIONS_H
#define ERROR_FUNCTIONS_H

#ifdef TRUE                                                                     
#undef TRUE                                                                     
#endif                                                                          
                                                                                
#ifdef FALSE                                                                    
#undef FALSE                                                                    
#endif                                                                          
                                                                                
typedef enum { FALSE, TRUE } Boolean; 

#ifdef __GNUC__
#define NORETURN __attribute__((__noreturn__))
#else
#define NORETURN
#endif

void cmdLineErr(const char* format, ...) NORETURN;
void errExit(const char* format, ...) NORETURN;
void fatal(const char* format, ...) NORETURN;
void usageError(const char* format, ...) NORETURN;

#endif
