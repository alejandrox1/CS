/* task.c
 *
 * Multitasking API.
 *
 * Taken from JamesM's kernel development tutorials.
 */
#ifndef _TASK_H
#define _TASK_H

#include "types.h"

// move_stack moves the current process' stack to a new location.
void move_stack(void *new_stack_start, uint32_t size);

#endif // _TASH_H
