#ifndef KASSERT_H
#define KASSERT_H

#include "common.h"
#include "monitor.h"

static void _monitor_trace(const char *file,  int32_t line, const char *msg)
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
#define KASSERT(expr) if (!(expr)) monitor_trace(ERRMSG)

#endif // KASSERT_H
