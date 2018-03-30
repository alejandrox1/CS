#include "kassert.h"
#include "types.h"
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
