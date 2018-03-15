/*
 * idt.c
 *
 * Implementation of the IDT.
 * Taken from JamesM's kernel development tutorial.
 */
#include "idt.h"
#include "monitor.h"


// IDT data structures.
idt_entry_t idt_entries[256];
idt_ptr_t idt_ptr;
interrupt_handler_t interrupt_handlers[256];


// idt_flush setting up the IDT must be done with assembly.
extern void idt_flush(uint32_t);

// idt_set_gate.
static void idt_set_gate(uint8_t, uint32_t, uint16_t, uint8_t);

/******************************************************************************
 *                             Public API                                     *
 *****************************************************************************/
/*
 * init_idt initializes the IDT and zeroes out all the ISRs.
 */
void init_idt()                                                                
{
	// Zero all interrupt handlers initially. 
	memset((uint8_t *)&interrupt_handlers, 0, sizeof(interrupt_handler_t)*256); 
	
	// Limit is set to the last valid byte in the IDT, after adding in the 
	// start position (i.e. size-1).                          
	idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;                               
	idt_ptr.base  = (uint32_t)&idt_entries; 
	
	// Zero the IDT to start with.
	memset((uint8_t *)&idt_entries, 0, sizeof(idt_entry_t)*255);
	
	/* 
	 * Set each gate in the IDT that we care about:
	 * 0-32:  Used by the CPU to report conditions, both normal and error.
	 * 255:   Will be used later as a way to execute system calls.
	 */
	idt_set_gate ( 0, (uint32_t)isr0 , 0x08, 0x8E);
	idt_set_gate ( 1, (uint32_t)isr1 , 0x08, 0x8E); 
	idt_set_gate ( 2, (uint32_t)isr2 , 0x08, 0x8E);   
	idt_set_gate ( 3, (uint32_t)isr3 , 0x08, 0x8E); 
	idt_set_gate ( 4, (uint32_t)isr4 , 0x08, 0x8E);
	idt_set_gate ( 5, (uint32_t)isr5 , 0x08, 0x8E);  
	idt_set_gate ( 6, (uint32_t)isr6 , 0x08, 0x8E);    
	idt_set_gate ( 7, (uint32_t)isr7 , 0x08, 0x8E);  
	idt_set_gate ( 8, (uint32_t)isr8 , 0x08, 0x8E);   
	idt_set_gate ( 9, (uint32_t)isr9 , 0x08, 0x8E);  
	idt_set_gate (10, (uint32_t)isr10, 0x08, 0x8E);  
	idt_set_gate (11, (uint32_t)isr11, 0x08, 0x8E);   
	idt_set_gate (12, (uint32_t)isr12, 0x08, 0x8E);  
	idt_set_gate (13, (uint32_t)isr13, 0x08, 0x8E);  
	idt_set_gate (14, (uint32_t)isr14, 0x08, 0x8E); 
	idt_set_gate (15, (uint32_t)isr15, 0x08, 0x8E);  
	idt_set_gate (16, (uint32_t)isr16, 0x08, 0x8E); 
	idt_set_gate (17, (uint32_t)isr17, 0x08, 0x8E); 
	idt_set_gate (18, (uint32_t)isr18, 0x08, 0x8E);  
	idt_set_gate (19, (uint32_t)isr19, 0x08, 0x8E); 
	idt_set_gate (20, (uint32_t)isr20, 0x08, 0x8E); 
	idt_set_gate (21, (uint32_t)isr21, 0x08, 0x8E); 
	idt_set_gate (22, (uint32_t)isr22, 0x08, 0x8E);  
	idt_set_gate (23, (uint32_t)isr23, 0x08, 0x8E); 
	idt_set_gate (24, (uint32_t)isr24, 0x08, 0x8E);
	idt_set_gate (25, (uint32_t)isr25, 0x08, 0x8E); 
	idt_set_gate (26, (uint32_t)isr26, 0x08, 0x8E);  
	idt_set_gate (27, (uint32_t)isr27, 0x08, 0x8E); 
	idt_set_gate (28, (uint32_t)isr28, 0x08, 0x8E);
	idt_set_gate (29, (uint32_t)isr29, 0x08, 0x8E); 
	idt_set_gate (30, (uint32_t)isr30, 0x08, 0x8E); 
	idt_set_gate (31, (uint32_t)isr31, 0x08, 0x8E);  
	idt_set_gate (255, (uint32_t)isr255, 0x08, 0x8E);  
	
	idt_flush((uint32_t)&idt_ptr);                                               
} 


/*
 * register_interrupt_hanlder
 */
void register_interrupt_handler(uint8_t n, interrupt_handler_t h)
{
	interrupt_handlers[n] = h;
}
/******************************************************************************
 *                             Private API                                    *
 *****************************************************************************/
/*
 * idt_set_gate
 */
static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
	idt_entries[num].base_lo = base & 0xFFFF;
	idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

	idt_entries[num].sel     = sel;
	idt_entries[num].always0 = 0;

	// Uncomment the OR below when we get to using user-mode. 
	// It sets the interrupt gate's privilege level to 3.
	idt_entries[num].flags   = flags /* | 0x60 */;
}


/******************************************************************************
 *                            idt.s helper methods                            *
 *****************************************************************************/
/*
 * idt_handler
 */
void idt_handler(registers_t *regs)
{
	if (interrupt_handlers[regs->int_no])
		interrupt_handlers[regs->int_no] (regs);
	else
	{
		monitor_write("unhandled interrupt: ");
		monitor_write_dec(regs->int_no);
		monitor_write("\n");
	}
}
