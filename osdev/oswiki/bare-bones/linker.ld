/* The bootloader will look at this image and start execution at the symbol
 * designated as the entrypoint. */
ENTRY(_start)

/* Tell where the various sections of the object files will be put in the final
 * kernel image. */
SECTIONS
{
    /* Begin putting sections at 1 MiB, a conventional place for kernels to be 
     * loaded t by the bootloader. */
    . = 1M;

    /*  First put the multiboot header, as it is required for this to be very
     *  early in the image or the bootloader won't recognize the file format.
     *  Next put the .text section. */
    .text BLOCK(4K) : ALIGN(4k)
    {
        *(.multiboot)
        *(.text)
    }

    /* Read-only data. */
    .rodata BLOCK(4k) : ALIGN(4K)
    {
        *(.rodata)
    }

    /* Read-write data (initialized). */
    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    /* Read-write data (uninitialized) and stack. */
    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }

    /* The compiler may produce other sections, by default it will put them in
     * a segment with the same name. Simply add here as needed. */
}
