/*
 * kheap.c
 *
 * Implementatin of the kernel heap.
 *
 * Taken from JamesM's kernel development tutorial.
 */
#include "common.h"
#include "kheap.c"

static int32_t find_smallest_hole(uint32_t size, uint8_t page_align, heap_t *heap);
/******************************************************************************
 *                                Public API                                  *
 *****************************************************************************/

/******************************************************************************
 *                                Private API                                 *
 *****************************************************************************/
/*
 * find_smallest_hole
 */
static int32_t find_smallest_hole(uint32_t size, uint8_t page_align, heap_t *heap)
{

}
