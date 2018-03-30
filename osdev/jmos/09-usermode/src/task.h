/* task.c
 *
 * Multitasking API.
 *
 * Taken from JamesM's kernel development tutorials.
 */
#ifndef _TASK_H
#define _TASK_H

#include "types.h"
#include "paging.h"

// task_t dfines a task (a process).
typedef struct task_struct
{
    int                 id;     
    uint32_t            esp;
    uint32_t            ebp;
    uint32_t            eip;
    page_directory_t   *page_directory;
    struct task_struct *next;
} task_t;

// intialise_tasking
void initialise_tasking();

// task_switch is called by a timer hook, this changes the running process.
void switch_task();

// fork forks the current process, spawning a new one with a different memory 
// space.
int fork();

// move_stack moves the current process' stack to a new location.
void move_stack(void *new_stack_start, uint32_t size);

// getpid returns the pid of the current process.
int getpid();


/* User Mode */
// switch_to_user_mode
void switch_to_user_mode();


#endif // _TASK_H
