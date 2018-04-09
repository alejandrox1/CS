/*
 * gdt.h
 *
 * Define interface for initializing the GDT and needed structures.
 * aken from JamesM' kernel development tutorial.
 */
#ifndef GDT_H
#define GDT_H

#include "types.h"

/*
 * gdt_entry_struct represents a GDT entry.
 * Attribute 'packed' tells gcc not to change any of the aligment in the
 * structure.
 */
typedef struct gdt_entry_struct
{
	uint16_t limit_low;    // Lower 16 bits of the segment limit.
	uint16_t base_low;     // Lower 16 bits of the segment's base.
	uint8_t  base_middle;  // Next 8 bits of the segment's base.
	uint8_t  access;       // Access flags, determine what ring this segment 
                           // can be used in.
    uint8_t  granularity;  // 
	uint8_t  base_high;    // Last 8 bits of the segment's base.
} __attribute__((packed)) gdt_entry_t;

/*
 * gdt_ptr_struct points to the start of our array of descriptor entries.
 * The format is required by the op lgdt.
 */
typedef struct gdt_ptr_struct
{
	uint16_t limit;    // Upper 16 bits of all selector limits.
	uint32_t base;     // Address of first gdt_entry_t struct.
} __attribute__((packed)) gdt_ptr_t;

/*
 * tss_entry_t describes a Task State Segment.
 */
typedef struct
{
    uint32_t prev_tss;   // The previous TSS - if we used hardware task switching this would form a linked list.
    uint32_t esp0;       // The stack pointer to load when we change to kernel mode.
    uint32_t ss0;        // The stack segment to load when we change to kernel mode.
    uint32_t esp1;       // Unused...
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t es;         // The value to load into ES when we change to kernel mode.
    uint32_t cs;         // The value to load into CS when we change to kernel mode.
    uint32_t ss;         // The value to load into SS when we change to kernel mode.
    uint32_t ds;         // The value to load into DS when we change to kernel mode.
    uint32_t fs;         // The value to load into FS when we change to kernel mode.
    uint32_t gs;         // The value to load into GS when we change to kernel mode.
    uint32_t ldt;        // Unused...
    uint16_t trap;
    uint16_t iomap_base;
} __attribute__((packed)) tss_entry_t;


// Initialization function.
void init_gdt();

// set_kernel_stack
void set_kernel_stack(uint32_t stack);


#endif
