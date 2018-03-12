/* 
 * main.c
 *
 * Define the kernel entrypoint and calls initialization routines.
 * From James Molloy kernel dev tutorial.
 */
#include "monitor.h"


int main(struct multiboot *mboot_ptr)
{
	monitor_clear();
	monitor_write("Hello, world!");

	// Alll initialization calls go in here.
	return 0xDEADBABA;
}
