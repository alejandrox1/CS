/* 
 * main.c
 *
 * Define the kernel entrypoint and calls initialization routines.
 * From James Molloy kernel dev tutorial.
 */
#include "common.h"
#include "gdt.h"
#include "idt.h"
#include "timer.h"
#include "paging.h"
#include "kheap.h"
#include "monitor.h"
#include "kassert.h"

static void int_3_handler(registers_t *regs)
{
	monitor_write("Interrupt 3 received!\n");
}


int main()//struct multiboot *mboot_ptr)
{

	/*
	 * Test monitor.h
	 */
	monitor_clear();
	monitor_write("Hello, world!");
	monitor_write("\nGoing to try and print a couple numbers:\n");
	
	monitor_write("\tprint 1 : ");
	monitor_write_dec(1);
	monitor_write("\tprint 10 : ");                                              
	monitor_write_dec(10);
	monitor_write("\tprint 110 : ");                                              
	monitor_write_dec(110);

	monitor_write("\n");

	monitor_write("\tprint 0x0 : ");                                              
	monitor_write_hex(0x0); 
	monitor_write("\tprint 0x5 : ");                                            
	monitor_write_hex(0x5);
	monitor_write("\tprint 0xA : ");                                             
	monitor_write_hex(0xA);                                                     
	monitor_write("\tprint 0xFF : ");                                            
	monitor_write_hex(0xFF);

	monitor_write("\n");

	/*
	 * Test IDT
	 */
	init_gdt();
	init_idt();

	register_interrupt_handler(3, &int_3_handler);

	asm volatile("int $0x3");
	asm volatile("int $0x4");

    /*
     * Test PIT api
     */
    /*
    init_timer(1); // 1 Hz
    
    asm volatile("sti");

    int i = 0;
    for (; i<5 ; i++);
    */
    /*
     * Paging
     */
    uint32_t a = kmalloc(8);
    initialise_paging();
    uint32_t b = kmalloc(8); 
    uint32_t c = kmalloc(8); 
    monitor_write("a: ");
    monitor_write_hex(a);
    monitor_write(", b: "); 
    monitor_write_hex(b); 
    monitor_write("\nc: "); 
    monitor_write_hex(c);
    
    kfree(c);
    kfree(b);
    uint32_t d = kmalloc(12);
    monitor_write(", d: ");
    monitor_write_hex(d);

	// All initialization calls go in here.
	return 0x12345677;
}
