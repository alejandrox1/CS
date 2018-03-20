/*
 * timer.c
 *
 * Initialize PIT, handle clock updates.
 * Taken from JamesM' kernel development tutorials.
 */
#include "common.h"
#include "timer.h"
#include "idt.h"
#include "monitor.h"

#define CLOCKF 1193180

uint32_t tick = 0;

// timer_callback
static void timer_callback(registers_t *regs);

/******************************************************************************
 *                                  Public API                                *
 *****************************************************************************/
/*
 * init_timer
 */
void init_timer(uint32_t frequency)
{
    // Register our timer callback func.
    register_interrupt_handler(IRQ0, &timer_callback);

    /*
     * The value we send to the PIT is the value to divide it's input clock
     * (1193180 Hz) by, to get our desired frequency. 
     * The divisor must fit in a 16-bit words.
     */
    uint32_t divisor = CLOCKF / frequency;

    // Send cmd byte (repeating mode).
    outb(0x43, 0x36);

    // Divisor must be sent byte-wise (upper/lower).
    uint8_t l = (uint8_t)(divisor & 0xFF);
    uint8_t h = (uint8_t)((divisor >> 8) & 0xFF);
    
    outb(0x40, l);
    outb(0x40, h);
}

/****************************************************************************** 
 *                                  Private API                               * 
 *****************************************************************************/
/*
 * timer_callback is an interrupt handler.
 */
static void timer_callback(registers_t *regs)
{
    tick++;
    if (tick%100 == 0)
    {
        monitor_write("Tick: ");
        monitor_write_dec(tick);
        monitor_write("\n");
    }
}
