/* Declare constants for the Multiboot header. */
.set ALIGN,     1<<0            /* Align loaded modules on page boundaries.     */
.set MEMINFO,   1<<1            /* Provide memory map.                          */
.set FLAGS,     ALIGN | MEMINFO /* Multiboot 'flag' field.                      */
.set MAGIC,     0x1BADB002      /* Magic number - helps bootloader find header. */
.set CHECKSUM, -(MAGIC + FLAGS) /* Checksum of above - prove we are Multiboot.  */

/*
Declare a multiboot header that marks the program as a kernel. These are magic 
values which are documented in the multiboot standard. The bootloader will 
search for this signature in the first 8 KiB of the kernel file, aligned at a 
32-bit boundary. The signature is in its own section so the header can be forced
to be within the first 8 KiB of the kernel file.
*/
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/*
The multiboot standard does not define the value of the stack pointer register
(ESP) and it is up to the kernel to provide a stack. This allocates room for a
small stack by creating a symbol at the bottom of it, then allocating 16384
bytes for it, and finally creating a symbol at the top. The stack grows downwards
on x86. The stack is in its own section so it can be marked nobits, which means
the kernel file is smaller because it does not contain an uninitialized stack.
The stack on x86 must be 16-byte aligned according to the System V ABI standard
and the de-facto extensions. The compiler will assume the stack is properly 
aligned and failure to align the stack will result in undefined behaviour.
*/
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

/*
The linker script specifies _start as the entry point to the kernel and the boot
loader will jump to this position once the kernel has been loaded. There won't 
be a need to return as the bootloader will be gone.
*/
.section .text
.global _start
.type _start, @function
_start:
    /*
    The bootloader has loaded us into 32-bit protected mode on x86. Interrupts
    are disabled. Paging is disabled. The processor state is as defined in the
    multiboot standard.
    */

    /*
    To set up a stack, we set the ESP register to point to the top of our stack.
    */
    mov $stack_top, %esp

    /*
    This is a googd place to initializecrucial processor state before the high-
    level kernel is entered. Note that the processor is not fully initialized 
    yet; features such as floating point instructions and instruction set 
    extensions are not initialized yet. The GDT should be loaded here. Paging 
    should be enabled here. C++ features such as global constructors and 
    exceptions will require runtime support to work as well.
    */

    /*
    Enter the high-level kernel. The ABI requires the stack is 16-byte aligned
    at the time of the call instruction (which afterwards pushes the return 
    pointer of size 4 bytes). The stack was originally 16-byte aligned above and
    we've since pushed a multiple of 16 bytes to the stack since (pushed 0 bytes
    so far) and the alignment is thus preserved and the call is well defined.
    */
    call kernel_main

    /*
    If the system has nothing more to do, send the computer into an infinite 
    loop:
    1) Disable interrupts with cli (clear interrupt enable in EFLAGS).
       They are already disabled by the bootloader, so this is not needed.
    2) Wait for the next interrupt to arrive with hlt (halt instruction).
       Since interrupts are disabled, this will lock the computer.
    3) Jump to the hlt instruction if it ever wakes up due to a non-masable 
       interrupt occurring or due to system management mode.
    */
    cli
1:  hlt
    jmp 1b

/*
et the size of the _start symbol to the current location '.' minus its start.
This is useful when debugging or when you implement call tracing.
*/
.size _start, . - _start
