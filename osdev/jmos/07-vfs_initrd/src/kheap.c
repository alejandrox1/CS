/*
 * kheap.c
 *
 * Implementatin of the kernel heap.
 *
 * Taken from JamesM's kernel development tutorial.
 */
#include "common.h"
#include "paging.h"
#include "kheap.h"
#include "kassert.h"


// Extern
// end is defined in the linker script.
extern uint32_t end;
uint32_t placement_address = (uint32_t)&end;
extern page_directory_t *kernel_directory;

// Global
heap_t *kheap = 0;


static int8_t   header_t_less_than(void *a, void *b);
static int32_t  find_smallest_hole(uint32_t size, uint8_t page_align, heap_t *heap);
static void     expand            (uint32_t new_size, heap_t *heap);
static uint32_t contract          (uint32_t new_size, heap_t *heap);
/******************************************************************************
 *                                Public API                                  *
 *****************************************************************************/
/*
 * kmalloc_int
 */
uint32_t kmalloc_int(uint32_t sz, uint8_t align, uint32_t *phys)
{
    if (kheap != 0)
    {
        void *addr = alloc(sz, align, kheap);
        if (phys != 0)
        {
            page_t *page = get_page((uint32_t)addr, 0, kernel_directory);
            *phys = (page->frame*0x1000) + ((uint32_t)addr&0xFFF);
        }
        return (uint32_t)addr;
    }
    else
    {
        if (align == 1 && (placement_address & 0xFFFFF000))
        {
             placement_address &= 0xFFFFF000;
             placement_address += 0x1000;
        }
        if (phys)
            *phys = placement_address;

        uint32_t tmp = placement_address;
        placement_address += sz;
        return tmp;
    }
}

/*
 * kmalloc
 */
uint32_t kmalloc(uint32_t sz)
{
    return kmalloc_int(sz, 0, 0);
}

/* 
 * kmalloc_a
 */
uint32_t kmalloc_a(uint32_t sz)
{ 
    return kmalloc_int(sz, 1, 0);
}

/*
 * kmalloc_p
 */
uint32_t kmalloc_p(uint32_t sz, uint32_t *phys)
{
    return kmalloc_int(sz, 0, phys);
}

/*
 * kmalloc_ap
 */
uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys)
{ 
    return kmalloc_int(sz, 1, phys);
}

/*
 * kfree
 */
void kfree(uint32_t p)
{
    free((void *)p, kheap);
}

/*
 * create_heap
 */
heap_t *create_heap(uint32_t start, uint32_t end, uint32_t max, uint8_t supervisor, uint8_t readonly)
{
    KASSERT(start%0x1000 == 0);
    KASSERT(end%0x1000 == 0);

    heap_t *heap = (heap_t *)kmalloc(sizeof(heap_t));

    // Initialize index.
    heap->index = place_ordered_array((void *)start, HEAP_INDEX_SIZE, &header_t_less_than);

    // Shift start address forward to where we can start putting in data (next
    // page boundary).
    start += sizeof(type_t)*HEAP_INDEX_SIZE;
    if ((start & 0xFFFFF000) != 0)
    {
        start &= 0xFFFFF000;
        start += 0x1000;
    }

    // Update heap's metadata.
    heap->start_address = start;
    heap->end_address   = end;
    heap->max_address   = max;
    heap->supervisor    = supervisor;
    heap->readonly      = readonly;

    // Place a large hole in the index.
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
    uint32_t new_size = size + sizeof(header_t) + sizeof(footer_t);
    int32_t  iterator = find_smallest_hole(new_size, page_align, heap);
    if (iterator == -1) // No hole is large enough.
    {
        // Save some data.
        uint32_t old_length      = heap->end_address - heap->start_address;
        uint32_t old_end_address = heap->end_address;

        // Allocate more space.
        expand(old_length+new_size, heap);
        uint32_t new_length = heap->end_address - heap->start_address;

        // Find the endmost header.
        iterator = 0;
        uint32_t idx = -1, value = 0x0;
        while (iterator < heap->index.size)
        {
            uint32_t tmp = (uint32_t)lookup_ordered_array(iterator, &heap->index);
            // Compare addresses.
            if (tmp > value)
            {
                value = tmp;
                idx = iterator;
            }
            iterator++;
        }

        // If no header found, then create a new one, else, adjust it.
        if (idx == -1) // When no holes in index.
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
            header_t *header  = lookup_ordered_array(idx, &heap->index);
            header->size     += new_length - old_length;
            
            // Rewrite footer.
            footer_t *footer  = (footer_t *)((uint32_t)header + header->size - sizeof(footer_t));
            footer->magic     = HEAP_MAGIC;
            footer->header    = header;
        }

        // We should now have enough space. Recurseee...
        return alloc(size, page_align, heap);
    }

    header_t *orig_hole_header = (header_t *)lookup_ordered_array(iterator, &heap->index);
    uint32_t orig_hole_pos     = (uint32_t)orig_hole_header;
    uint32_t orig_hole_size    = orig_hole_header->size;
    // Is it worth splitting into two? 
    // will the free space be able to fit another hole in there? 
    // If not, then increase the request size so as to not split.
    // Is the requested hole size less than the overhead for adding a new hole?
    // If so then split in two.
    if (orig_hole_size - new_size < sizeof(header_t) + sizeof(footer_t))
    {
        // Increase the requested size to the size of the hole we found.
        size     += orig_hole_size - new_size;
        new_size  = orig_hole_size;
    }

    // If we need to pre-alignd the data, do it now and make a new hole in
    // front of our block. Reuse the old hole, else remove it.
    if (page_align && orig_hole_pos&0xFFFFF000)
    {
        uint32_t new_location = orig_hole_pos + 0x1000 - (orig_hole_pos&0xFFF) - sizeof(header_t);
        
        header_t *hole_header = (header_t *)orig_hole_pos;
        hole_header->size     = 0x1000 - (orig_hole_pos&0xFFF) - sizeof(header_t);
        hole_header->magic    = HEAP_MAGIC;
        hole_header->is_hole  = 1;

        footer_t *hole_footer = (footer_t *)((uint32_t)new_location - sizeof(footer_t));
        hole_footer->magic    = HEAP_MAGIC;
        hole_footer->header   = hole_header;
       
        orig_hole_pos  = new_location;
        orig_hole_size = orig_hole_size - hole_header->size;
    }
    else
    {
        // We don't need the hoel anymore, delete it from index.
        remove_ordered_array(iterator, &heap->index);
    }

    // Overwrite the original header.
    header_t *block_header = (header_t *)orig_hole_pos;
    block_header->magic    = HEAP_MAGIC;
    block_header->is_hole  = 0;
    block_header->size     = new_size;
    // Overwrite the original footer.
    footer_t *block_footer = (footer_t *)(orig_hole_pos + sizeof(header_t) + size);
    block_footer->magic    = HEAP_MAGIC;
    block_footer->header   = block_header;

    // Split hole in two?
    // We may need to write a new hole after the allocated block if the new
    // hole could have positive size.
    if (orig_hole_size - new_size > 0)
    {
        header_t *hole_header = (header_t *)(orig_hole_pos + sizeof(header_t) + size + sizeof(footer_t));
        hole_header->magic    = HEAP_MAGIC;
        hole_header->is_hole  = 1;
        hole_header->size     = orig_hole_size - new_size;

        footer_t *hole_footer = (footer_t *)((uint32_t)hole_header + orig_hole_size - new_size - sizeof(footer_t));
        if ((uint32_t)hole_footer < heap->end_address)
        {
            hole_footer->magic  = HEAP_MAGIC;
            hole_footer->header = hole_header;
        }
        // Put new hole in index.
        insert_ordered_array((void *)hole_header, &heap->index);
    }

    return (void *)((uint32_t)block_header + sizeof(header_t));
}

/*
 * free
 */
void free(void *p, heap_t *heap)
{
    if (!p) 
        return;

    header_t *header = (header_t *)((uint32_t)p - sizeof(header_t));
    footer_t *footer = (footer_t *)((uint32_t)header + header->size - sizeof(footer_t));
    KASSERT(header->magic == HEAP_MAGIC);
    KASSERT(footer->magic == HEAP_MAGIC);

    // Make a hole.
    header->is_hole = 1;
    // Do we want to add it to the index?
    char do_add = 1;

    // Unify left, if the thing inmediately to the left of us is a footer.
    footer_t *test_footer = (footer_t *)((uint32_t)header - sizeof(footer_t));
    if (test_footer->magic == HEAP_MAGIC && test_footer->header->is_hole == 1)
    {
        uint32_t cache_size = header->size;        // Cache current header size.
        header              = test_footer->header; // Rewrite our header.
        footer->header      = header;              // Point to new header.
        header->size       += cache_size;          
        do_add              = 0;
    }

    // Unify right, If inmediately to the right we have a header.
    header_t *test_header = (header_t *)((uint32_t)footer + sizeof(footer_t));
    if (test_header->magic == HEAP_MAGIC && test_header->is_hole == 1)
    {
        header->size += test_header->size;
        test_footer   = (footer_t *)((uint32_t)test_header + test_header->size - sizeof(footer_t));
        footer        = test_footer;

        // Find and remove the header from the index.
        uint32_t iterator = 0;
        while ((iterator < heap->index.size) && 
                (lookup_ordered_array(iterator, &heap->index) != (void *)test_header))
            iterator++;
        KASSERT(iterator < heap->index.size);

        remove_ordered_array(iterator, &heap->index);
    }

    // COntrat if footer is at the end of index.
    if ((uint32_t)footer+sizeof(footer_t) == heap->end_address)
    {
        uint32_t old_length = heap->end_address - heap->start_address;
        uint32_t new_length = contract((uint32_t)header - heap->start_address, heap);
        if (header->size - (old_length - new_length) > 0)
        {
            // We still exist, resize us.
            header->size  -= old_length - new_length;
            footer         = (footer_t *)((uint32_t)header + header->size - sizeof(footer_t));
            footer->magic  = HEAP_MAGIC;
            footer->header = header;
        }
        else
        {
            // We no longer exist, rm us from index.
            uint32_t iterator = 0;
            while ((iterator < heap->index.size) && 
                    (lookup_ordered_array(iterator, &heap->index) != (void *)test_header))
                iterator++;

            if (iterator < heap->index.size)
                remove_ordered_array(iterator, &heap->index);
        }
    }

    // Add yourself!
    if (do_add == 1)
        insert_ordered_array((void *)header, &heap->index);
}



/******************************************************************************
 *                                Private API                                 *
 *****************************************************************************/
/*
 * header_t_less_than
 */
static int8_t header_t_less_than(void *a, void *b)
{
    return ( ((header_t *)a)->size < ((header_t *)b)->size)?1:0;
}

/*
 * find_smallest_hole
 */
static int32_t find_smallest_hole(uint32_t size, uint8_t page_align, heap_t *heap)
{
    uint32_t iterator = 0;
    while (iterator < heap->index.size)
    {
        header_t *header = (header_t *)lookup_ordered_array(iterator, &heap->index);
        if (page_align > 0)
        {
            // align the memroy the 'user' has access to, not the header.
            // Page align the starting point of the header.
            uint32_t location = (uint32_t)header;
            int32_t offset    = 0;
            if (((location+sizeof(header_t)) & 0xFFFFF000) != 0) 
                offset = 0x1000 - (location+sizeof(header_t))%0x1000; 
            
            int32_t hole_size = (int32_t)header->size - offset;
            // Hole is larger than requested size.
            if (hole_size >= (int32_t)size)
                break;
        }
        // Item at given position is larger than requested size.
        else if (header->size >= size)
            break;

        iterator++;
    }

    // Why did the loop exit?
    if (iterator == heap->index.size)
        return -1;
    return iterator;
}

/*
 * expand
 */
static void expand(uint32_t new_size, heap_t *heap)
{
    KASSERT(new_size > heap->end_address - heap->start_address);

    // Get nearest following page boundary.
    if ((new_size & 0xFFFFF000) != 0)
    {
        new_size &= 0xFFFFF000;
        new_size += 0x1000;
    }
    KASSERT(heap->start_address + new_size <= heap->max_address);

    // Page boundaries!
    uint32_t old_size = heap->end_address - heap->start_address;
    uint32_t i = old_size;
    while (i < new_size)
    {
        alloc_frame(get_page(heap->start_address+i, 1, kernel_directory), 
                (heap->supervisor)?1:0, (heap->readonly)?0:1);
        i += 0x1000;
    }
    heap->end_address = heap->start_address + new_size;
}

/*
 * contract
 */
static uint32_t contract(uint32_t new_size, heap_t *heap)
{
    KASSERT(new_size < heap->end_address - heap->start_address);

    // Get nearest following page boundary.
    if (new_size&0x1000)
    {
        new_size &= 0x1000;
        new_size += 0x1000;
    }

    // Don't overdue it.
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
