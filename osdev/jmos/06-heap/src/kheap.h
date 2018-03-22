/*
 * kheap.c
 *
 * Taken from JamesM' kernel development tutorial.
 */
#ifndef KHEAP_H
#define KHEAP_H

#include "common.h"
#include "ord_array.h"


#define KHEAP_START 0xC0000000
#define KHEAP_INITIAL_SIZE 0x100000
#define HEAP_INDEX_SIZE 0x20000
#define HEAP_MAGIC 0x123890AB
#define HEAP_MIN_SIZE 0x70000

/*
 * header_t for holes and blocks.
 */
typedef struct
{
    uint32_t magic;
    uint8_t  is_hole; 
    uint32_t size;
} header_t;

/*
 * footer_t for holes and blocks.
 */
typedef struct
{
    uint32_t magic;
    header_t *header;
} footer_t;

/*
 * heap_t
 */
typedef struct
{
    ordered_array_t index;
    uint32_t start_address;
    uint32_t end_address;    // May be expanded up to max_address.
    uint32_t max_address;
    uint8_t supervisor;      // Extra pages mapped to supervisor?
    uint8_t readonly;        // Extra pages mapped to read-only?
} heap_t;


// create_heap.
heap_t *create_heap(uint32_t start, uint32_t end, uint32_t max, uint8_t supervisor, uint8_t readonly);

// alloc allocates a contiguous region of memory size. If page_align==1, it
// creates a block on a page boundary.
void *alloc(uint32_t size, uint8_t page_align, heap_t *heap);

// free releases a block allocated with alloc.
void free(void *p, heap_t *heap);



#endif
