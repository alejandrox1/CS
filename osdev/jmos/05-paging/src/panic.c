#include "panic.h"
#include "monitor.h"


void panic(const char *msg)
{
    monitor_write("\n*** System panic: ");
    monitor_write(msg);
    monitor_write("***\n");
    for (;;);
}
