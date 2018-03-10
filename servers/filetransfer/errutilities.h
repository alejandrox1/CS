#ifndef __ERRUTILITIES_H__
#define __ERRUTILITIES_H__

#include <errno.h>
#include <string.h>

#define ERRMSG 1024


// getError returns error message.
void getError(int n, char* errmsg)
{
	// TODO: better error handling here... 
	strerror_r(n, errmsg, ERRMSG);
}


#endif // __ERRUTILITIES_H__
