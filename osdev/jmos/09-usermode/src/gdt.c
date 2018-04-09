/*
 * gdt.c 
 *
 * Initializae the GDT.
 * Taken from JamesM' kernel development tutorial.
 */
#include "types.h"
#include "gdt.h"

#define ENTRIES 6

// gdt_entries is the GDT.
gdt_entry_t gdt_entries[ENTRIES];
// gdt_ptr points to the GDT.
gdt_ptr_t gdt_ptr;


// gdt_flush sets up the GDT. Call to load GDT must be done in assembly.
extern void gdt_flush(uint32_t);
// tss_flush
extern void tss_flush();

// gdt_set_gate
static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);
// write_tss 
static void wite_tss(int32_t num, uint16_t ss0, uint32_t esp0);

/******************************************************************************
 *                              Public API                                    *
 *****************************************************************************/
/*
 * init_gdt initializes the GDT.
 */
void init_gdt()
{
	/* 
	 * We have 6 (ENTRIES) entries in the GDT - 2 for kernel mode, 2 for user 
	 * mode, 1 for the NULL descriptor, and 1 for the TSS (Task State Segment).
	 */
	
	// The limit is the last valid byte from the start of the GDT - size of the
	// GDT minus 1.
	gdt_ptr.limit = sizeof(gdt_entry_t) * ENTRIES - 1;
	gdt_ptr.base = (uint32_t)&gdt_entries;
	
	gdt_set_gate(0, 0, 0, 0, 0);             // Null segment.                    
	gdt_set_gate(1, 0, 0xFFFFF, 0x9A, 0xCF); // Code segment.                    
	gdt_set_gate(2, 0, 0xFFFFF, 0x92, 0xCF); // Data segment.                    
	gdt_set_gate(3, 0, 0xFFFFF, 0xFA, 0xCF); // User mode code segment.
	//gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

	gdt_flush((uint32_t)&gdt_ptr);
}

/*
 * set_kernel_stack
 */
void set_kernel_stack(uint32_t stack)
{
    return;
}

/******************************************************************************
 *                              Private API                                   *
 *****************************************************************************/
/*
 * gdt_set_gate
 *
 * Parameters
 * ----------
 *  num    : entry in GDT (gdt_entries[]).
 *  base   : descriptor's base.
 *  limit  : descriptor's limit.
 *  access : ring level.
 *  gran   : granularity.
 */
static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
	gdt_entries[num].base_low    = base & 0xFFFF;
	gdt_entries[num].base_middle = (base >> 16) & 0xFF;
	gdt_entries[num].base_high   = (base >> 24) & 0xFF;

	gdt_entries[num].limit_low   = limit & 0xFFFF;
	gdt_entries[num].granularity = (limit >> 16) & 0x0F;

	gdt_entries[num].granularity |= gran & 0xF0;
	gdt_entries[num].access      =  access;
}

/* 
 * write_tss initialises our tast state segment struct.
 */
static void write_tss(int32_t num, uint16_t ss0, uint32_t esp0)
{
    // Compute the base and limit of our entry into the GDT.
    uint32_t base  = (uint32_t)&tss_entry;
    uint32_t limit = base + sizeof(tss_entry);

    // Add TSS descriptor's address into the GDT.
    get_set_gate(num, base, limit, 0xE9, 0x00);

    // Ensure the descriptor is initially zero
    memset((uint8_t *)&tss_entry, 0, sizeof(tss_entry));

    tss_entry.ss0  = ss0;  // Set the kernel stack segment.
    tss_entry.esp0 = esp0; // Set the kernel stack pointer.

    /*
     * Set cs, ds, ss, es, fs, and gs entries in the TSS. These specify what
     * segments should be loaded when the processor switches to kernel mode.
     * They are our normal kernel code/data segments, 0x08 and 0x10
     * respectively, with the last 2 bits set, resulting in 0x0b and 0x13.
     * Setting these bits sets the RPL (request privilege level) to 3, meaning
     * this TSS can be used to switch to kernel mode from ring 3.
     */
    tss_entry.cs = 0x0b;
    tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x13;
}
