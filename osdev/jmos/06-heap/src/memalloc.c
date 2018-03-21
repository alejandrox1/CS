#include "memalloc.h"

// Defined in kheap.c
extern uint32_t placement_address;


/******************************************************************************
 *                             Public API                                     *
 ******************************************************************************/
/*
 * kmalloc vanila memory allocator.
 */
uint32_t kmalloc(uint32_t sz)
{
    uint32_t tmp = placement_address;
    placement_address += sz;
    return tmp;
}

/*
 * kmalloc_a allocates page-aligned memory.
 */
uint32_t kmalloc_a(uint32_t sz)
{
    if ((placement_address & 0xFFFFF000)) // Address not already aligned.
    {
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }
    uint32_t tmp = kmalloc(sz);
    return tmp;
}

/*
 * kmalloc_p returns the physical address of the allocated memory.
 */
uint32_t kmalloc_p(uint32_t sz, uint32_t *phys)
{
    if (phys)
        *phys = placement_address;

    uint32_t tmp = kmalloc(sz);
    return tmp;
}

/*
 * kmalloc_ap returns the physical address of the newly page-aligned allocated
 * memeory.
 */
uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys)
{
    if ((placement_address & 0xFFFFF000)) // Address not already aligned.
    {
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }
    if (phys)
        *phys = placement_address;

    uint32_t tmp = kmalloc(sz);
    return tmp;
}
