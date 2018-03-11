/* 
 * main.c
 *
 * Define the kernel entrypoint and calls initialization routines.
 * From James Molloy kernel dev tutorial.
 */

int main(struct multiboot *mboot_ptr)
{
	// Alll initialization calls go in here.
	return 0xDEADBABA;
}
