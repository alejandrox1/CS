/*
 * gdt.h
 *
 * Define interface for initializing the GDT and needed structures.
 * aken from JamesM' kernel development tutorial.
 */
#ifndef GDT_H
#define GDT_H

#include "common.h"

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



// Initialization function.
void init_gdt();


#endif
