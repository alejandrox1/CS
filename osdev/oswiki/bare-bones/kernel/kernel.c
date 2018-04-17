#include <vga.h>

#if defined(__linux__)
#error "Cross-compiler is not being used"
#endif

#if !defined(__i386__)
#error "This needs to be compiled for a 32-bit ix86 target"
#endif

void kernel_main(void)
{
    terminal_initialize();
    terminal_writestring("Hello, kernel world!\n");
}
