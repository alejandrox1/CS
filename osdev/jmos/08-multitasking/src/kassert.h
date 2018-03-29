#ifndef KASSERT_H
#define KASSERT_H

#include "types.h"

void _monitor_trace(const char *file,  int32_t line, const char *msg);

#define monitor_trace(msg) _monitor_trace(__FILE__, __LINE__, (msg))

#define ERRMSG "Error occurred -"                                               
#define KASSERT(expr) if (!(expr)) monitor_trace(ERRMSG)

#endif // KASSERT_H
