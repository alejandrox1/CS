/* task.c
 *
 * Multitasking API implementation.
 *
 * Taken from JamesM's kernel development tutorials.
 */
#include "task.h"
#include "common.h"
#include "gdt.h"
#include "paging.h"
#include "kheap.h"
#include "monitor.h"

extern page_directory_t *kernel_directory;
extern page_directory_t *current_directory;
extern uint32_t          initial_esp;
extern uint32_t          read_eip();
extern void              perform_task_switch(uint32_t, uint32_t, uint32_t, uint32_t);

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
    current_task->ebp            = 0;
    current_task->eip            = 0;
    current_task->page_directory = current_directory;
    current_task->next           = 0;

    // Get them interrupts back.
    asm volatile("sti");
}

/*
 * switch_task
 */
void switch_task()
{
    // Make sure we already initialise_tasking()
    if (!current_task)
        return;

    // We will need these...
    uint32_t esp, ebp, eip;
    asm volatile("mov %%esp, %0" : "=r" (esp));
    asm volatile("mov %%ebp, %0" : "=r" (ebp));
    
    /*
     * Read the intruction pointer and...
     * One of two things coul've happened when read_eip() exits:
     *  1. We called the function and it returned the EIP as requested.
     *  2. We have just switched tasks, and because the saved EIP is the
     *     instruction after read_eip(), it will seem as if it has just 
     *     returned.
     * In the second case we need to return inmediately.
     * To detect this case, we place a dummy value in EAX further down at the
     * end of this function (return value will be the dummy 0x12345).
     */
    eip = read_eip();
    // Did we jus switch tasks?
    if (eip == 0x12345)
        return;

    // We did NOt switch tasks: lets' save some stuff.
    current_task->eip = eip;
    current_task->esp = esp;
    current_task->ebp = ebp;

    // Get next task to run.
    current_task = current_task->next;
    // If current_task wen't NULL then go around.
    if (!current_task) current_task = ready_queue;

    eip = current_task->eip;
    esp = current_task->esp;
    ebp = current_task->ebp;

    // Make sure the memory manager knows we changed page directory.
    current_directory = current_task->page_directory;

    /*
     * - Temporarily put EIP in ECX.
     * - Load stack and base pointer for new stack.
     * - Change page directory.
     * - Put dummy return value (0x12345) in EAX to recognize we just switched 
     *   tasks.
     * - Jump to next location in ECX (EIP is in there).
     */
    perform_task_switch(eip, current_directory->physicalAddr, ebp, esp);
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
    task_t *new_task = (task_t *)kmalloc(sizeof(task_t));
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
    for (i = (uint32_t)new_stack_start; i >= ((uint32_t)new_stack_start - size); i -= 0x1000)
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

/*
 * getpid
 */
int getpid()
{
    return current_task->id;
}


/*
 * switch_to_user_mode
 */
void switch_to_user_mode()
{
    // Set up the kernel stack.
    set_kernel_stack(current_task->kernel_stack + KERNEL_STACK_SIZE);

    // Set up the stack structure for switching to user mode.
    asm volatile("          \
            cli;            \
            mov $0x23, %ax; \
            mov %ax, %ds;   \
            mov %ax, %es;   \
            mov %ax, %fs;   \
            mov %ax, %gs;   \
                            \
            mov %esp, %eax; \
            pushl $0x23;    \
            pushl %esp;     \
            pushf;          \
            pushl $0x1B;    \
            push $1f;       \
            iret;           \
        1:                  \
            ");
}
/******************************************************************************
 *                               Private API                                  *
 *****************************************************************************/
