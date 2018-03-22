/*
 * kheap.c
 *
 * Taken from JamesM' kernel development tutorial.
 */
#ifndef KHEAP_H
#define KHEAP_H

#include "common.h"
#include "ord_array.h"
#include "kinterfaces.h"


#define KHEAP_START 0xC0000000
#define KHEAP_INITIAL_SIZE 0x100000
#define HEAP_INDEX_SIZE 0x20000
#define HEAP_MAGIC 0x123890AB
#define HEAP_MIN_SIZE 0x70000

typedef struct
{
    uint32_t             heap_start_address; // must be initialized, must be greater than 0   
    uint32_t             hole_start_address; // must be initialized, must be greater than heap_start_address
    uint32_t             heap_end_address;   // must be initialized, must be greater than hole_start_address, must be 0 modulo 0x1000
    uint32_t             heap_stop_address;  // must be initialized, must be greater than heap_end_address, must be 0 modulo 0x1000
    pager_t              pager;              // all fields must be initialized                             
    free_space_indexer_t free_space_index;   // all fields except address_begin, address_end must be initialized
} heap_params_t ;

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
//heap_t *create_heap(uint32_t start, uint32_t end, uint32_t max, uint8_t supervisor, uint8_t readonly);
void create_heap(heap_params_t *heap_params);

// alloc allocates a contiguous region of memory size. If page_align==1, it
// creates a block on a page boundary.
void *alloc(uint32_t size, uint8_t page_align, heap_t *heap);

// free releases a block allocated with alloc.
void free(void *p, heap_t *heap);


uint32_t kmalloc_int(uint32_t sz, int32_t align, uint32_t *phys);
uint32_t kmalloc(uint32_t sz);
uint32_t kmalloc_a(uint32_t sz);
uint32_t kmalloc_p(uint32_t sz, uint32_t *phys);
uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys);
void kfree(uint32_t p);


#endif // KHEAP_H
