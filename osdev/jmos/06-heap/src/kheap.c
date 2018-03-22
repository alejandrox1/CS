/*
 * kheap.c
 *
 * Taken from James' kernel development tutorials.
 */
#include "kheap.h"
#include "paging.h"
#include "memalloc.h"
#include "common-io.h"

// exertern
heap_t *kheap;
extern page_directory_t *kernel_directory;

static int8_t header_t_less_than(void *a, void *b);
static uint32_t find_smallest_hole(uint32_t size, uint8_t page_align, heap_t *heap);
static void expand(uint32_t new_size, heap_t *heap);
static uint32_t contract(uint32_t new_size, heap_t *heap);

/******************************************************************************
 *                                Public API                                  *
 *****************************************************************************/
/*
 * create_heap
 */
heap_t *create_heap(uint32_t start, uint32_t end, uint32_t max, uint8_t supervisor, uint8_t readonly)
{
    heap_t *heap = (heap_t *)kmalloc(sizeof(heap_t));

    // We assume start and end addresses are being page-aligned.
    ASSERT(start%0x1000 == 0);
    ASSERT(end%0x1000 == 0);

    // Initialise the index.
    heap->index = place_ordered_array((void *)start, HEAP_INDEX_SIZE, &header_t_less_than);
    
    // Shift the start address forward to resemble where we can start putting
    // data.
    start += sizeof(type_t)*HEAP_INDEX_SIZE;

    // Make sure the start address is page-aligned.
    if ((start & 0xFFFFF000) != 0)
    {
        start &= 0xFFFFF000;
        start += 0x1000;
    }

    // Write the start, end, and max addresses into the heap structure.
    heap->start_address = start;
    heap->end_address   = end;
    heap->max_address   = max;
    heap->supervisor    = supervisor;
    heap->readonly      = readonly;

    // Create one large hole in the index.
    header_t *hole = (header_t *)start;
    hole->size     = end - start;
    hole->magic    = HEAP_MAGIC;
    hole->is_hole  = 1;
    insert_ordered_array((void *)hole, &heap->index);

    return heap;
}

/*
 * alloc
 */
void *alloc(uint32_t size, uint8_t page_align, heap_t *heap)
{
    // Take the size of the header and footer into account.
    uint32_t new_size = size + sizeof(header_t) + sizeof(footer_t);
    // Find the smallest hole.
    int32_t iterator = find_smallest_hole(new_size, page_align, heap);
    // No good hole.
    if (iterator == -1)
    {
        /*
         * No large enough hole was found, expand the size of the heap. Account
         * for expansion in the index by finding the endmost hole and adjusting
         * its size. 
         * The only time this won't work is if there aren't any holes
         * in the index at all. In such a case make one to fill the gap.
         */
        // Save some data.
        uint32_t old_length = heap->end_address - heap->start_address;
        uint32_t old_end_address = heap->end_address;

        // Allocate more space.
        expand(old_length+new_size, heap);
        uint32_t new_length = heap->end_address - heap->start_address;

        // Find the endmost header.
        iterator = 0;
        // Index and value of endmost header thus far.
        uint32_t idx = -1, value = 0x0;
        while (iterator < heap->index.size)
        {
            uint32_t tmp = (uint32_t)lookup_ordered_array(iterator, &heap->index);
            if (tmp > value)
            {
                value = tmp;
                idx = iterator;
            }
            iterator++;
        }

        // Add a header if none found.
        if (idx == -1)
        {
            header_t *header = (header_t *)old_end_address;
            header->magic    = HEAP_MAGIC;
            header->size     = new_length - old_length;
            header->is_hole  = 1;

            footer_t *footer = (footer_t *)(old_end_address + header->size - sizeof(footer_t));
            footer->magic    = HEAP_MAGIC;
            footer->header   = header;

            insert_ordered_array((void *)header, &heap->index);
        }
        else
        {
            // Last header needs adjusting.
            header_t *header = lookup_ordered_array(idx, &heap->index);
            header->size    += new_length - old_length;
            
            // Rewirte footer.
            footer_t *footer = (footer_t *)((uint32_t)header + header->size - sizeof(footer_t));
            footer->header   = header;
            footer->magic    = HEAP_MAGIC;
        }

        // We should now have enough space. Try again.
        return alloc(size, page_align, heap);
    }

    header_t *orig_hole_header = (header_t *)lookup_ordered_array(iterator, &heap->index);
    uint32_t orig_hole_pos = (uint32_t)orig_hole_header;
    uint32_t orig_hole_size = orig_hole_header->size;
    // Split hole? Is the requested hole size less than the overhead for adding
    // a new hole? Will the free space be able to fit another hole into it? If
    // not, increase the requested size to the whole size so not to split.
    if (orig_hole_size-new_size < sizeof(header_t)+sizeof(footer_t))
    {
        // Increase the requested hole size to the size of the hole we found.
        size += orig_hole_size - new_size;
        new_size = orig_hole_size;
    }

    // If we need to page-align then do it and make a new hole in front of our
    // block. We will reuse the original hole in this case.
    if (page_align && orig_hole_pos&0xFFFFF000)
    {
        uint32_t new_location = orig_hole_pos + 0x1000 /* page size */ - (orig_hole_pos&0xFFF) - sizeof(header_t);
        
        header_t *hole_header = (header_t *)orig_hole_pos;
        hole_header->size     = 0x1000 /* page size */ - (orig_hole_pos&0xFFF) - sizeof(header_t);
        hole_header->magic    = HEAP_MAGIC;
        hole_header->is_hole  = 1;

        footer_t *hole_footer = (footer_t *)((uint32_t)new_location - sizeof(footer_t));
        hole_footer->magic    = HEAP_MAGIC;
        hole_footer->header   = hole_header;

        orig_hole_pos         = new_location;
        orig_hole_size        = orig_hole_size - hole_header->size;
    }
    else
    {
        // We don't need this hole, delete it.
        remove_ordered_array(iterator, &heap->index);
    }

    // Overwrite the original header.
    header_t *block_header = (header_t *)orig_hole_pos;
    block_header->magic    = HEAP_MAGIC;
    block_header->is_hole  = 0;
    block_header->size     = new_size;
    // Overwrite the footer.
    footer_t *block_footer = (footer_t *)(orig_hole_pos + sizeof(header_t) + size);
    block_footer->magic    = HEAP_MAGIC;
    block_footer->header   = block_header;

    // If we want to split our hole in two.
    // We may need to write a new hole after the allocated block. We do this if
    // the new hole were to have positive size.
    if (orig_hole_size - new_size > 0)
    {
        header_t *hole_header = (header_t *)(orig_hole_pos + sizeof(header_t) + size + sizeof(footer_t));
        hole_header->magic    = HEAP_MAGIC;
        hole_header->is_hole  = 1;
        hole_header->size     = orig_hole_size - new_size;

        footer_t *hole_footer = (footer_t *)((uint32_t)hole_header + orig_hole_size - new_size - sizeof(footer_t));
        if ((uint32_t)hole_footer < heap->end_address)
        {
            hole_footer->magic = HEAP_MAGIC;
            hole_footer->header = hole_header;
        }
        // Put new hole in index.
        insert_ordered_array((void *)hole_header, &heap->index);
    }

        return (void *)((uint32_t)block_header+sizeof(header_t));
}

/*
 * free
 */
void free(void *p, heap_t *heap)
{
    // Exit gracefully for null poitners.
    if (!p)
        return;

    // Get header and footer associated with this pointer.
    header_t *header = (header_t *)((uint32_t)p - sizeof(header_t));
    footer_t *footer = (footer_t *)((uint32_t)header + header->size - sizeof(footer_t));
    ASSERT(header->magic == HEAP_MAGIC);
    ASSERT(footer->magic == HEAP_MAGIC);

    // Make a hole.
    header->is_hole = 1;

    // Add hole to index?
    char do_add = 1;

    // Unify left.
    // Check to the left to see if there is a footer.
    footer_t *test_footer = (footer_t *)((uint32_t)header - sizeof(footer_t));
    if (test_footer->magic == HEAP_MAGIC && test_footer->header->is_hole == 1)
    {
        uint32_t cache_size = header->size;    // Cache our current size.
        header = test_footer->header;          // Rewrite our header with new one.
        footer->header = header;               // Update our footer.
        header->size += cache_size;            // Update size.
        do_add = 0;
    }

    // Unify right.
    // Check to the right to see if there is a header.
    header_t *test_header = (header_t *)((uint32_t)footer + sizeof(footer_t));
    if (test_footer->magic == HEAP_MAGIC && test_header->is_hole)
    {
        header->size += test_header->size;    // Update size.
        // Rewrite footer to point to our header.
        test_footer = (footer_t *)((uint32_t)test_header + test_header->size + sizeof(footer_t));
        footer = test_footer;

        // Find and remove header to the right from index.
        uint32_t iterator = 0;
        while ((iterator < heap->index.size) && 
                (lookup_ordered_array(iterator, &heap->index) != (void *)test_header))
            iterator++;
        ASSERT(iterator < heap->index.size);
        remove_ordered_array(iterator, &heap->index);
    }

    // If footer is at end address, we can contract.
    if ((uint32_t)footer+sizeof(footer_t) == heap->end_address)
    {
        uint32_t old_length = heap->end_address - heap->start_address;
        // contract() will either contract and get rid of our hole (else
        // clause), or partially contract, or maybe do nothing.
        uint32_t new_length = contract((uint32_t)header - heap->start_address, heap);
        // Check size after resizing.
        if (header->size - (old_length - new_length) > 0)
        {
            // We still exists, so resize us.
            header->size -= old_length - new_length;
            footer = (footer_t *)((uint32_t)header + header->size - sizeof(footer_t));
            footer->magic = HEAP_MAGIC;
            footer->header = header;
        }
        else
        {
            // We don't exist anymore, so rm us.
            uint32_t iterator = 0;
            while ((iterator < heap->index.size) &&
                    (lookup_ordered_array(iterator, &heap->index) != (void *)test_header))
                iterator++;
            if (iterator < heap->index.size)
                remove_ordered_array(iterator, &heap->index);
        }
    }

    if (do_add == 1)
        insert_ordered_array((void *)header, &heap->index);
}
/******************************************************************************
 *                               Private API                                  *
 *****************************************************************************/
/*
 * header_t_less_than sort by size instead of pointer address.
 */
static int8_t header_t_less_than(void *a, void *b)
{
    return (((header_t *)a)->size < ((header_t *)b)->size)?1:0;
}

/*
 * find_smallest_hole
 */
static uint32_t find_smallest_hole(uint32_t size, uint8_t page_align, heap_t *heap)
{
    //Find smallest hole.
    uint32_t iterator = 0;
    while (iterator < heap->index.size)
    {
        header_t *header = (header_t *)lookup_ordered_array(iterator, &heap->index);
        if (page_align != 0)
        {
            // Page align the header's starting address.
            uint32_t location = (uint32_t)header;
            int32_t  offset   = 0;
            if (((location+sizeof(header_t)) & 0xFFFFF000) != 0)
                offset = 0x1000 /* page size */ - (location+sizeof(header_t))%0x1000;
            
            int32_t hole_size = (int32_t)header->size - offset;
            // Does it fit?
            if (hole_size >= (int32_t)size)
                break;
        }
        else if (header->size >= size)
            break;
        iterator++;
    }
    // Why did the loop exit?
    if (iterator == heap->index.size)
        return (uint32_t)-1;
    else
        return iterator;
}

/*
 * expand
 */
static void expand(uint32_t new_size, heap_t *heap)
{
    ASSERT(new_size > heap->end_address - heap->start_address);

    // Get nearest page boundary.
    if ((new_size&0xFFFFF000) != 0)
    {
        new_size &= 0xFFFFF000;
        new_size += 0x1000;
    }

    // Make sure we are not overeaching.
    ASSERT(heap->start_address + new_size <= heap->max_address);

    // This should always be on a page boudnary.
    uint32_t old_size = heap->end_address - heap->start_address;
    uint32_t i = old_size;
    while (i <new_size)
    {
        alloc_frame(get_page(heap->start_address+i, 1, kernel_directory), 
                (heap->supervisor)?1:0, (heap->readonly)?1:0);
        i += 0x1000;
    }
    heap->end_address = heap->start_address + new_size;
}

/*
 * contratc
 */
static uint32_t contract(uint32_t new_size, heap_t *heap)
{
    ASSERT(new_size < heap->end_address - heap->start_address);

    // Get the nearest page boundary.
    if (new_size & 0x1000)
    {
        new_size &= 0x1000;
        new_size += 0x1000;
    }

    // Don't contract too far.
    if (new_size < HEAP_MIN_SIZE)
        new_size = HEAP_MIN_SIZE;

    uint32_t old_size = heap->end_address - heap->start_address;
    uint32_t i = old_size - 0x1000;
    while (new_size < i)
    {
        free_frame(get_page(heap->start_address+i, 0, kernel_directory));
        i -= 0x1000;
    }
    heap->end_address = heap->start_address + new_size;
    return new_size;
}
