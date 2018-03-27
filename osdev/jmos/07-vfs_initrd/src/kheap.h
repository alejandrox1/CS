/*
 * kheap.h
 *
 * Kernel's heap API.
 *
 * Taken from JamesM's kernel development tutorial.
 */
#ifndef KHEAP_H
#define KHEAP_H

#include "common.h"
#include "ord_array.h"


#define KHEAP_START         0xC0000000 // 3GB == 12* 16**7 / (1024**3)
#define KHEAP_INITIAL_SIZE  0x100000   // 1MB
#define HEAP_INDEX_SIZE     0x20000    // 2MB
#define HEAP_MAGIC          0x123890AB 
#define HEAP_MIN_SIZE       0x70000    // 7MB

/* 
 * header_t 
 */
typedef struct 
{
    uint32_t magic;
    uint8_t  is_hole;
    uint32_t size;
} header_t;

/*
 * footer_t
 */
typedef struct
{
    uint32_t magic;
    header_t *header;
} footer_t;

typedef struct
{
    ordered_array_t index;
    uint32_t        start_address;
    uint32_t        end_address;
    uint32_t        max_address;
    uint8_t         supervisor;
    uint8_t         readonly;
} heap_t;


// create_heap.
heap_t *create_heap(uint32_t start, uint32_t end, uint32_t max, uint8_t supervisor, uint8_t readonly);

// alloc allocates a contiguous region of memory. If page_aligned, then it will
// create that block on a page boundary.
void *alloc(uint32_t size, uint8_t page_align, heap_t *heap);

// free releases a block allocated with alloc.
void free(void *p, heap_t *heap);


/*
 * Memory Allocation Functions.
 */
// kmalloc_int allocates a chunk of memory of size sz. If align == 1, the chunk
// will be page aligned. If phys != 0, the physical location of the allocated
// chunk will be stored in phys.
uint32_t kmalloc_int(uint32_t sz, uint8_t align, uint32_t *phys);

// kmalloc is a general purpose memory allocation function.
uint32_t kmalloc(uint32_t sz);

// kmalloc_a will page aligned the allocated memory.
uint32_t kmalloc_a(uint32_t sz);

// kmalloc_p saves the physical address.
uint32_t kmalloc_p(uint32_t sz, uint32_t *phys);

// kmalloc_ap page aligns the allocated memory and saves physical address.
uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys);

// kfree is a general purpose memory deallocation function.
void kfree(uint32_t p);


#endif // KHEAP_H
