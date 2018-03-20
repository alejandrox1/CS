#include "panic.h"
#include "monitor.h"


void panic(char *msg)
{
    monitor_write("*** System panic: ");
    monitor_write(msg);
    monitor_write("***\n");
    for (;;);
}
