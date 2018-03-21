#include "common.h"
#include "common-io.h"
#include "paging.h"
#include "memalloc.h"
#include "monitor.h"
#include "panic.h"


#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))


// Bit set of frames.
uint32_t *frames;
uint32_t nframes;
page_directory_t *current_directory, *kernel_directory;

// Defined in kheap.c
uint32_t placement_address = 0x0E00000;

void page_fault(registers_t *regs);

// set_frame sets a bit in the frames bitset.
static void set_frame(uint32_t frame_addr);
// clear_frame clears a bit from the frames bitset.
static void clear_frame(uint32_t frame_addr);
// test_frame tests if a bit is set in the frames bitset.
static uint32_t test_frame(uint32_t frame_addr);
// first_frame finds the first free frame.
static uint32_t first_frame();
// alloc_frame allocates a frame.
void alloc_frame(page_t *page, int32_t is_kernel, int32_t is_writeable);
// free_frame deallocates frame.
void free_frame(page_t *page);

/****************************************************************************** 
 *                                Public API                                  *
 ******************************************************************************/
/*
 * initialise_paging.
 * Create the frames bitset and initialises it to 0.
 * It allocates page-aligned space for a page_directory_t.
 * It allocates frames such that any page access will map will map to the frame
 * with the same linear address - identity mapping.
 * Register an interrupt handler for page faults and then enables paging.
 */
void initialise_paging()
{
    // For the moment, we assume the physical memory is is 16MB big.
    uint32_t mem_end_page = 0x1000000;

    nframes = mem_end_page / 0x1000;
    frames = (uint32_t*)kmalloc(INDEX_FROM_BIT(nframes));
    memset((uint8_t *)frames, 0, INDEX_FROM_BIT(nframes));

    // Make a page directory.
    kernel_directory = (page_directory_t *)kmalloc_a(sizeof(page_directory_t));
    memset((uint8_t *)kernel_directory, 0, sizeof(page_directory_t));
    current_directory = kernel_directory;

    /*
     * We need to identify map (phys addr == virt addr) from 0x0 to the end of
     * used memory, in order to transparently access it - as if paging wasn't
     * enabled.
     * Inside the loop below, we will change placement_address by calling
     * kmalloc(). A while loop causes this to be computed on-the-fly rather
     * than once at start.
     */
    uint32_t i = 0;
    while (i < placement_address)
    {
        // Kernel code is readable but not writeable from user-mode.
        alloc_frame( get_page(i, 1, kernel_directory), 0, 0);
        i += 0x1000;
    }
    // Before enabling paging, we must register a page fault handler.
    register_interrupt_handler(14, page_fault);

    // Enable paging.
    switch_page_directory(kernel_directory);
}

/*
 * switch_page_directory
 */
void switch_page_directory(page_directory_t *dir)
{
    current_directory = dir;
    asm volatile("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
    uint32_t cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    
    // Enable paging and flushes page-directory cache.
    cr0 |= 0x80000000; 
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}

/*
 * get_page returns a pointer to the page entry for a particular address.
 * If make is 1, and the page table that the requested page entry should reside
 * hasn't been created, then it will be created,otherwise return 0.
 * Of table has already been assigned, it will return the page entry and return
 * it, otherwise (make==1) will attempt to make it.
 */
page_t *get_page(uint32_t address, int make, page_directory_t *dir)
{
    // Turn address into index.
    address /= 0x1000;

    // Find page table containing this address.
    uint32_t table_idx = address / 1024;
    uint32_t page_idx = address % 1024;
    if (dir->tables[table_idx]) // If table is already assigned.
        return &dir->tables[table_idx]->pages[page_idx];
    else if (make)
    {
        uint32_t tmp;
        dir->tables[table_idx] = (page_table_t *)kmalloc_ap(sizeof(page_table_t), &tmp);
        memset((uint8_t *)dir->tables[table_idx], 0, 0x1000);
        dir->tablesPhysical[table_idx] = tmp | 0x7;    // present, rw, us.
        return &dir->tables[table_idx]->pages[page_idx];
    }
    else
        return 0;
}

/*
 * page_fault
 */
void page_fault(registers_t *regs)                                             
{
    // Fault address is stored in cr2.
    uint32_t cr2;
    asm volatile ("mov %%cr2, %0" : "=r" (cr2));  
    
    // Error code gives us info on what happened.
    uint32_t present  = !(regs->err_code & 0x1);  // Page not present
    uint32_t rw       = regs->err_code & 0x2;     // Write operation?
    uint32_t us       = regs->err_code & 0x4;     // Processor was in user-mode?
    uint32_t reserved = regs->err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
    uint32_t id       = regs->err_code & 0x10;    // Caused by an instruction fetch?

    // Output an error message.
    monitor_write("Page fault! ( ");
    if (present) {monitor_write("present ");}
    if (rw) {monitor_write("read-only ");}
    if (us) {monitor_write("user-mode ");}
    if (reserved) {monitor_write("reserved ");}
    if (id) {monitor_write("instruction ftech ");}
    monitor_write(") at 0x");
    monitor_write_hex(cr2);
    monitor_write("\n");
    
    monitor_write("Page fault at 0x");
    monitor_write_hex(regs->eip);
    monitor_write(", faulting address 0x");
    monitor_write_hex(cr2); 
    monitor_write("\nError code: ");
    monitor_write_hex(regs->err_code); 

    monitor_trace("\nTrace");
    panic(" ");
    for (;;) ;                                                                    
} 

/******************************************************************************
 *                                Private API                                 *
 ******************************************************************************/
/*
 * set_frame sets a bit in the frames bitset.
 */
static void set_frame(uint32_t frame_addr)
{
    uint32_t frame = frame_addr / 0x1000;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);
    frames[idx] |= (0x1 << off);
}

/*
 * clear_frame clears a bit from the frames bitset.
 */
static void clear_frame(uint32_t frame_addr)
{
    uint32_t frame = frame_addr / 0x1000;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);
    frames[idx] &= ~(0x1 << off);
}

/*
 * test_frame tests if a bit is set in the frames bitset.
 */
static uint32_t test_frame(uint32_t frame_addr)
{
    uint32_t frame = frame_addr / 0x1000;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);
    return (frames[idx] & (0x1 << off));
}

/*
 * first_frame finds the first free frame.
 */
static uint32_t first_frame()
{
    uint32_t i, j;
    for (i = 0; i < INDEX_FROM_BIT(nframes); i++)
    {
        if (frames[i] != 0xFFFFFFFF) // Nothing free, exit early.
        {
            // At least one bit is set free here.
            for (j = 0; j < 32; j++)
            {
                uint32_t toTest = 0x1 << j;
                if ( !(frames[i] & toTest) )
                    return i * 4 * 8 + j;
            }
        }
    }
    return (uint32_t)-1;
}


/*
 * alloc_frame allocates a frame.
 */
void alloc_frame(page_t *page, int32_t is_kernel, int32_t is_writeable)
{
    if (page->frame != 0)
        return; // Frame was already allocated, return right away.
    else
    {
        uint32_t idx = first_frame();
        if (idx == (uint32_t)-1)
            panic("No free frames!");
        
        // This page is ours.
        set_frame(idx * 0x1000);
        page->present = 1;       
        page->rw      = (is_writeable)?1:0;
        page->user    = (is_kernel)?0:1;
        page->frame   = idx;
    }
}

/*
 * free_frame deallocates frame.
 */
void free_frame(page_t *page)
{
    uint32_t frame;
    if (!(frame = page->frame))
        return; // The given page doesn't actually have an allocated frame.
    else
    {
        clear_frame(frame);
        page->frame = 0x0;
    }
}
