#ifndef COMMONIO_H
#define COMMONIO_H

#include "common.h"
#include "monitor.h"

void _monitor_trace(const char *file,  int32_t line, const char *msg)
{
    monitor_write(msg);
    monitor_write(" ");
    monitor_write(file);
    monitor_write(":");
    monitor_write_dec(line);
    monitor_write("\n");
}

#define monitor_trace(msg) _monitor_trace(__FILE__, __LINE__, (msg))

#define ERRMSG "Error occurred -"                                               
#define ASSERT(expr) if (!(expr)) monitor_trace(ERRMSG)

#endif
