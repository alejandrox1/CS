/* task.c
 *
 * Multitasking API implementation.
 *
 * Taken from JamesM's kernel development tutorials.
 */
#include "task.h"
#include "common.h"
#include "paging.h"

extern page_directory_t *kernel_directory;
extern page_directory_t *current_directory;
extern uint32_t          initial_esp;

/******************************************************************************
 *                               Public API                                   *
 *****************************************************************************/
/*
 * move_stack
 */
void move_stack(void *new_stack_start, uint32_t size)
{
    uint32_t i;
    // Allocate space for the new stack.
    for ( i = (uint32_t)new_stack_start; i >= ((uint32_t)new_stack_start - size); i -= 0x1000)
    {
        // General-purpose stack is in user-mode.
        alloc_frame( get_page(i, 1, current_directory), 0 /* user-mode */, 1 /* writable */ );
    }

    // We changed a page table so we need to inform the processor that mapping
    // has changed. For this, we need to flush the TLB - read & write the page
    // directory address again.
    uint32_t pd_addr;
    asm volatile("mov %%cr3, %0" : "=r" (pd_addr));
    asm volatile("mov %0, %%cr3" : : "r" (pd_addr));

    // Read current stack and base pointers, calculate an offset to get from an
    // address in the old stack to an address in the new stack.
    uint32_t old_stack_pointer;
    uint32_t old_base_pointer;
    asm volatile("mov %%esp, %0" : "=r" (old_stack_pointer));
    asm volatile("mov %%ebp, %0" : "=r" (old_base_pointer));

    uint32_t offset = (uint32_t)new_stack_start - initial_esp;
    uint32_t new_stack_pointer = old_stack_pointer + offset;
    uint32_t new_base_pointer  = old_base_pointer  + offset;

    // Copy the stack!
    memcpy((void *)new_stack_pointer, (void *)old_stack_pointer, initial_esp-old_stack_pointer);

    // Backtrace through the original stack, copying new values into the new
    // stack.
    // Look for base pointers to change: any value in the range of the stack
    // (old_stack_pointer < x < initial_esp).
    for (i = (uint32_t)new_stack_start; i > (uint32_t)new_stack_start-size; i -= 4)
    {
        uint32_t tmp = *(uint32_t *)i;
        // If value of tmp is in range of the old stack, assume it is a base
        // pointer and remap it. This will, however, remap ANY value in this
        // range, whether they are base pointers or not.
        if ((old_stack_pointer < tmp) && (tmp < initial_esp))
        {
            tmp = tmp + offset;
            uint32_t *tmp2 = (uint32_t *)i;
            *tmp2 = tmp;
        }
    }

    // Change stack and base pointers.
    asm volatile("mov %0, %%esp" : : "r" (new_stack_pointer));
    asm volatile("mov %0, %%ebp" : : "r" (new_base_pointer));
}


/******************************************************************************
 *                               Private API                                  *
 *****************************************************************************/
