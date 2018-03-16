/*
 * idt.h
 *
 * Define the IDT.
 * Taken from JamesM's kernel development tutorials.
 */
#ifndef IDT_H
#define IDT_H

#include "common.h"

#define ISR3 3
#define IRQ0 32


/*
 * idt_entry_struct describes an interrupt gate.
 */
typedef struct idt_entry_struct
{
	uint16_t base_lo;    // Lower 16 bits of address to jmp to.
	uint16_t sel;        // Kernel segment selector.
	uint8_t  always0;    // Must always be zero.
	uint8_t  flags;      
	uint16_t base_hi;    // Upper 16 bits of address to jmp to.
} __attribute__((packed)) idt_entry_t;


/*
 * idt_ptr_struct defines a pointer to an array of interrupt handlers.
 * Format prescribed by op lidt.
 */
typedef struct idt_ptr_struct
{
	uint16_t limit;    
	uint32_t base;    // Addrs of 1st element in idt_entry_t array.
} __attribute__((packed)) idt_ptr_t;

/*
 * registers_struct contains the registers value when cpu was interrupted.
 */
typedef struct registers_struct
{
	uint32_t ds;  
	uint32_t edi, esi, ebp, esp, eax, ebx, ecx, edx; // Pushed by pusha.
	uint32_t int_no, err_code; 
	uint32_t eip, cs, ss, eflags, useresp; // Pushed by processor automatically.
} registers_t;

// interrupt_handler_t represents an interrupt handler. It is a function
// pointer which takes a pointer to a structure containing register values.
typedef void (*interrupt_handler_t)(registers_t *);



// init_idt initializes the IDT.
void init_idt();
// register_interrupt_handler an interrupt handler.                             
void register_interrupt_handler(uint8_t n, interrupt_handler_t h);


// These extern directives let us access the addresses of our ISR handlers. 
extern void isr0 ();                                                            
extern void isr1 ();                                                            
extern void isr2 ();                                                            
extern void isr3 ();                                                            
extern void isr4 ();                                                            
extern void isr5 ();                                                            
extern void isr6 ();                                                            
extern void isr7 ();                                                            
extern void isr8 ();                                                            
extern void isr9 ();                                                            
extern void isr10();                                                            
extern void isr11();                                                            
extern void isr12();                                                            
extern void isr13();                                                            
extern void isr14();                                                            
extern void isr15();                                                            
extern void isr16();                                                            
extern void isr17();                                                            
extern void isr18();                                                            
extern void isr19();                                                            
extern void isr20();                                                            
extern void isr21();                                                            
extern void isr22();                                                            
extern void isr23();                                                            
extern void isr24();                                                            
extern void isr25();                                                            
extern void isr26();                                                            
extern void isr27();                                                            
extern void isr28();                                                            
extern void isr29();                                                            
extern void isr30();                                                            
extern void isr31();                                                            
extern void isr255();
extern void irq0 ();                                                            
extern void irq1 ();                                                            
extern void irq2 ();                                                            
extern void irq3 ();                                                            
extern void irq4 ();                                                            
extern void irq5 ();                                                            
extern void irq6 ();                                                            
extern void irq7 ();                                                            
extern void irq8 ();                                                            
extern void irq9 ();                                                            
extern void irq10();                                                            
extern void irq11();                                                            
extern void irq12();                                                            
extern void irq13();                                                            
extern void irq14();                                                            
extern void irq15();


#endif
