/*
 * syscall.c
 *
 * Implementation of the syscall API.
 *
 * Taken from JamesM's kernel development tutorials.
 */
#include "types.h"
#include "syscall.h"
#include "idt.h"
#include "monitor.h"


DEFN_SYSCALL1(monitor_write,     0, const char*);
DEFN_SYSCALL1(monitor_write_hex, 1, const char*);
DEFN_SYSCALL1(monitor_write_dec, 2, const char*);


uint32_t num_syscalls = 3;
static void *syscalls[3] =
{
    &monitor_write,
    &monitor_write_hex,
    &monitor_write_dec,
};

static void syscall_handler(registers_t *regs);
/******************************************************************************
 *                                 Public API                                 *
 *****************************************************************************/
/*
 * initialise_syscalls registers a handler for the interrupt 0x80.
 */
void initialise_syscalls()
{
    register_interrupt_handler(0x80, &syscall_handler);
}
/******************************************************************************
 *                                 Private API                                *
 *****************************************************************************/
/*
 * syscall_handler
 */
static void syscall_handler(registers_t *regs)
{
    // Check if requested syscall number (in EAX) is valid.
    if (regs->eax >= num_syscalls)
        return;

    // Get syscall location.
    void *location = syscalls[regs->eax];

    // We don't know how many arguments the function wants, so simply push them
    // all onto the stack.
    int ret;
    asm volatile("     \
            push %1;   \
            push %2;   \
            push %3;   \
            push %4;   \
            push %5;   \
            call *%6;  \
            pop %%ebx; \
            pop %%ebx; \
            pop %%ebx; \
            pop %%ebx; \
            pop %%ebx; \
            " : "=a"(ret) : "r"(regs->edi), "r"(regs->esi), "r"(regs->edx), "r"(regs->ecx), "r"(regs->ebx), "r"(location));
    // Save return value on EAX.
    regs->eax = ret;
}

