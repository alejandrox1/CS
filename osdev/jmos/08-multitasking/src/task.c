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
extern uint32_t          read_eip();

volatile task_t *current_task;
volatile task_t *ready_queue;   // Start or task linked list.
uint32_t         next_pid = 1;


/******************************************************************************
 *                               Public API                                   *
 *****************************************************************************/
/*
 * initialise_tasking
 */
void initialise_tasking()
{
    // Rather important stuff happening, no interrupts please!
    asm volatile("cli");

    // Rellocate the stack just to now where it is.
    move_stack((void *)0xE0000000, 0x2000);

    // Initialise the first stack (kernel stack).
    current_task = ready_queue = (task_t *)kmalloc(sizeof(task_t));
    current_task->id             = next_pid++;
    current_task->esp            = 0;
    current-Task->ebp            = 0;
    current_task->eip            = 0;
    current_task->page_directory = current_directory;
    current_task->next           = 0;

    // Get them interrupts back.
    asm volatile("sti");
}

/*
 * fork
 */
int fork()
{
    // We are going to modify some kernel structures...
    asm volatile("cli");

    // Take a pointer to this process' task struct for later reference.
    task_t *parent_task = (task_t *)current_task;

    // Clone address space.
    page_directory_t *directory = clone_directory(current_directory);

    // Create a new process.
    task_t new_task = (task_t *)kmalloc(sizeof(task_t));
    new_task->id             = next_pid++;
    new_task->esp            = 0;
    new_task->ebp            = 0;
    new_task->eip            = 0;
    new_task->page_directory = directory;
    new_task->next           = 0;

    // Add it to the end of the ready queue.
    task_t *tmp_task = (task_t *)ready_queue;
    while (tmp_task->next)
        tmp_task = tmp_task->next;
    tmp_task->next = new_task;

    // This will be the entrypoint for the new process.
    uint32_t eip = read_eip();

    // Check whether we are the parent or the child.
    if (current_task == parent_task)
    {
        // We are the parent, so set up ESP, EBP, and EIP for child.
        uint32_t esp, ebp;
        asm volatile("mov %%esp, %0" : "=r" (esp));
        asm volatile("mov %%ebp, %0" : "=r" (ebp));
        new_task->esp = esp;
        new_task->ebp = ebp;
        new_task->eip = eip;
        asm volatile("sti");

        return new_task->id;
    }
    else
    {
        // We are the child.
        return 0;
    }
}

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
