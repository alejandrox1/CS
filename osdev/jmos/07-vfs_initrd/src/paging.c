#include "common.h"
#include "paging.h"
#include "kheap.h"
#include "free_space_indexer.h"
#include "monitor.h"
#include "panic.h"
#include "kassert.h"


#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

# define HEAP_SPEC_HOLE_MAXIMUM_SIZE 0x300000                                   
# define HEAP_SPEC_HOLE_MINIMUM_SIZE 0x100000                                   
# define HEAP_SPEC_INDEX_SIZE        0x20000                                    
# define HEAP_SPEC_INITIAL_SIZE (HEAP_SPEC_INDEX_SIZE+HEAP_SPEC_HOLE_MINIMUM_SIZE)
# define HEAP_SPEC_MAXIMUM_SIZE (HEAP_SPEC_INDEX_SIZE+HEAP_SPEC_HOLE_MAXIMUM_SIZE)
# define HEAP_SPEC_START_ADDRESS 0xC0000000                                     
# define HEAP_SPEC_HOLE_START_ADDRESS (HEAP_SPEC_START_ADDRESS + HEAP_SPEC_INDEX_SIZE)
# define HEAP_SPEC_END_ADDRESS  (HEAP_SPEC_START_ADDRESS + HEAP_SPEC_INITIAL_SIZE)
# define HEAP_SPEC_STOP_ADDRESS (HEAP_SPEC_START_ADDRESS + HEAP_SPEC_MAXIMUM_SIZE)
# define HEAP_SPEC_SUPERVISOR 0                                                 
# define HEAP_SPEC_READONLY 0 

// Extern
extern uint32_t placement_address; 
void copy_page_data(uint32_t src, uint32_t dst); // copy_page_data.s

// Globals
// Bit set of frames.
static uint32_t *frames;
static uint32_t nframes;
static page_directory_t *kernel_directory = 0;
page_directory_t *current_directory = 0;


// set_frame sets a bit in the frames bitset.
static void set_frame(uint32_t frame_addr);
// clear_frame clears a bit from the frames bitset.
static void clear_frame(uint32_t frame_addr);
// test_frame tests if a bit is set in the frames bitset.
uint32_t test_frame(uint32_t frame_addr);
// first_frame finds the first free frame.
static uint32_t first_frame();
// alloc_frame allocates a frame.
void alloc_frame(page_t *page, int32_t is_kernel, int32_t is_writeable);
// free_frame deallocates frame.
void free_frame(page_t *page);

static uint32_t pager_get_page_impl(pager_t *pager, uint32_t address, int32_t make);
static void pager_alloc_frame_impl(pager_t *pager, uint32_t page);
static void pager_free_frame_impl(pager_t *pager, uint32_t page);
static uint32_t pager_build_physical_address_impl(pager_t *pager, uint32_t page, uint32_t address);
static void pager_impl_export(pager_t *pager, page_directory_t *dir, int32_t supervisor, int32_t readonly);

static page_table_t *clone_table(page_table_t *src, uint32_t *physAddr);

/****************************************************************************** 
 *                                Public API                                  *
 ******************************************************************************/
/*
 * initialize_paging.
 * Create the frames bitset and initialises it to 0.
 * It allocates page-aligned space for a page_directory_t.
 * It allocates frames such that any page access will map will map to the frame
 * with the same linear address - identity mapping.
 * Register an interrupt handler for page faults and then enables paging.
 */
void initialize_paging()
{
    // For the moment, we assume the physical memory is is (0x1000000) 16MB big.
    uint32_t mem_end_page = 0x8000000;

    nframes = mem_end_page / 0x1000;
    frames = (uint32_t *)kmalloc(4*INDEX_FROM_BIT(nframes));
    memset((uint8_t *)frames, 0, 4*INDEX_FROM_BIT(nframes));

    // This should be framed when we use it.
    heap_params_t *heap_params = (heap_params_t *)kmalloc(sizeof(heap_params_t));

    // Make a page directory.
    kernel_directory = (page_directory_t *)kmalloc_a(sizeof(page_directory_t));
    memset((uint8_t *)kernel_directory, 0, sizeof(page_directory_t));
    kernel_directory->physicalAddr = ((uint32_t)kernel_directory->tablesPhysical);

    /*
     * Map some pages in the kernel heap area.
     * Call get_page but not alloc_frame. This will cause page_table_t's to be
     * created where necessary. We can't allocate frames yet because they need
     * to be indentity mapped (see below), and we can't increase the placement
     * address between identity mapping and enabling he heap.
     */
    uint32_t fa = HEAP_SPEC_START_ADDRESS;
    uint32_t re = HEAP_SPEC_END_ADDRESS; 
    while ( re>fa ) 
    { 
        (void) get_page (fa, 1, kernel_directory); 
        fa += 0x1000 ;
    }
    
    /*
     * We need to identity map (phys addr == virt addr) from 0x0 to the end of
     * used memory, in order to transparently access it - as if paging wasn't
     * enabled.
     * Inside the loop below, we will change placement_address by calling
     * kmalloc(). A while loop causes this to be computed on-the-fly rather
     * than once at start.
     */
    fa = 0;
    while ( placement_address + 0x1000 > fa ) 
    {  
        // Kernel code is readable but not writeable from user space. 
        // --> get_page (address , make , page_directory )   
        page_t * page = get_page (fa, 1, kernel_directory ); 
        // --> alloc_page (page , is_kernel , is_writeable ) 
        alloc_frame ( page, 0, 0 );  
        // Increase fa to the next page address.
        fa += 0x1000; 
    }

    // Now allocate those pages we mapped earlier.
    re = HEAP_SPEC_END_ADDRESS; 
    for ( fa = HEAP_SPEC_START_ADDRESS; re > fa; fa+=0x1000 ) 
    {
        page_t * page = get_page( fa, 1, kernel_directory );
        alloc_frame(page, 0, 0);
    }

    // Before enabling paging, we must register a page fault handler.
    register_interrupt_handler(14, page_fault);

    // Enable paging.
    switch_page_directory(kernel_directory);

    // Switch heaps' impl's operations.
    heap_params->heap_start_address = HEAP_SPEC_START_ADDRESS;
    heap_params->hole_start_address = HEAP_SPEC_HOLE_START_ADDRESS;
    heap_params->heap_end_address   = HEAP_SPEC_END_ADDRESS;
    heap_params->heap_stop_address  = HEAP_SPEC_STOP_ADDRESS;
    pager_impl_export(&heap_params->pager, kernel_directory, HEAP_SPEC_SUPERVISOR, HEAP_SPEC_READONLY);
    free_space_indexer_impl_export(&heap_params->free_space_index);
    create_heap(heap_params);
    
    current_directory = clone_directory(kernel_directory );
    switch_page_directory(current_directory);
}

/*
 * switch_page_directory
 */
void switch_page_directory(page_directory_t *dir)
{
    current_directory = dir;
    asm volatile("mov %0, %%cr3" : : "r" (dir->physicalAddr));
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
page_t *get_page(uint32_t address, int32_t make, page_directory_t *dir)
{
    // Turn address into index.
    address /= 0x1000;

    // Find page table containing this address.
    uint32_t table_idx = address / 1024;
    if (dir->tables[table_idx] != 0) // If table is already assigned.
        return &dir->tables[table_idx]->pages[address % 1024];
    
    if (make != 0)
    {
        uint32_t tmp;
        dir->tables[table_idx] = (page_table_t *)kmalloc_ap(sizeof(page_table_t), &tmp);
        ASSERT((0xFFF&tmp) == 0);
        memset((uint8_t *)dir->tables[table_idx], 0, 0x1000);
        dir->tablesPhysical[table_idx] = tmp | 0x7;    // present, rw, us.
        return &dir->tables[table_idx]->pages[address % 1024];
    }
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

/*
 * clone_directory
 */
page_directory_t *clone_directory(page_directory_t *src) 
{ 
    // u32int esp ; asm volatile ( "mov %%esp, %0" : "=r" (esp)) ;  
    // page_t * pesp = get_page ( esp , 0 , current_directory ) ; 
    // u32int esp_phys = pesp->frame * 0x1000 + (esp%1024);   
    // monitor_write ( "esp: " ) ; monitor_write_hex (esp); monitor_write ( " " ) ;
    // monitor_write ( "at: " ) ; monitor_write_hex (esp_phys); monitor_write ( "\n" ) ;
  
    uint32_t phys ; // physical address of the new page directory   
    page_directory_t *dir = (page_directory_t*)kmalloc_ap(sizeof(page_directory_t), &phys);
    memset((uint8_t *)dir, 0, sizeof(page_directory_t));  
  
    // Get the physical address of tablesPhysical    
    uint32_t offset = ((uint32_t)dir->tablesPhysical)-((uint32_t)dir); 
    uint32_t physic = phys + offset ; // by accident , not good code here    
    // monitor_write ( "tables physical (1): " ) ; monitor_write_hex (physic); monitor_write ("\n" ) ;

    // correct is to do like this  
    uint32_t address = (uint32_t)dir->tablesPhysical;  
    page_t * page = get_page(address, 0, kernel_directory);      
    uint32_t physical = (page->frame * 0x1000) + (0xFFF & address);   
    // monitor_write ( "tables physical (2): " ) ; monitor_write_hex(physical);monitor_write ("\n" ) ;
    ASSERT(physic == physical);   

    dir->physicalAddr=physical; 
    // u32int lool = (u32int) (& dir->physicalAddr ) ;    
    // page_t * palool = get_page ( lool, 0, current_directory ) ;      
    // u32int phylool = palool->frame*0x1000 + (0xfff&lool);        
    // monitor_write("lool: " ) ; monitor_write_hex (phylool); monitor_write ("\n");
    // monitor_write ( "zzz: " ) ; monitor_write_hex(dir->physicalAddr);monitor_write ("\n" ) ;
    
    // Pages 
    int32_t fa; 
    // u32int ca ; 
    for (fa = 0; fa < 1024; ++fa) 
    {  
        if (src->tables[fa] == 0) 
        continue;

        // We link kernel pages, and copy non-kernel pages from src. 
        if ((kernel_directory->tables[fa] == src->tables[fa])) 
        {   
            // ca = 1 ;  
            dir->tables[fa]         = src->tables[fa];  
            dir->tablesPhysical[fa] = src->tablesPhysical[fa]; 
        } 
        else 
        {  
            // ca = 2 ;  
            uint32_t phys;   
            dir->tables[fa] = clone_table(src->tables[fa], &phys); 
            ASSERT((0xFFF&phys) == 0);  
            dir->tablesPhysical[fa] = phys|0x07;  
        } 
        // monitor_write ( " " ) ; monitor_write_dec (ca); monitor_write ( " " ) ;
        // monitor_write_hex(fa);monitor_write (" " ) ;  
        // monitor_write_hex ( src->tablesPhysical[fa]) ; monitor_write ( " " ) ;
        // monitor_write_hex ( dir->tablesPhysical[fa]) ;  
        // monitor_write ( " " ) ; monitor_write_hex (dir->physicalAddr);  
        // monitor_write ( " " ) ; monitor_write_hex ((u32int)dir); 
        // monitor_write ( "\n" ) ;  
    }
  return dir;
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
uint32_t test_frame(uint32_t frame_addr)
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
    for (i = 0; i < INDEX_FROM_BIT(nframes); ++i)
    {
        if (frames[i] != 0xFFFFFFFF) // Nothing free, exit early.
        {
            // At least one bit is set free here.
            for (j = 0; j < 32; ++j)
            {
                if ( (frames[i] & (0x1 << j)) == 0)
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
    ASSERT(page->frame == 0);
    if (page->frame != 0)
        return; // Frame was already allocated, return right away.
    
    uint32_t idx = first_frame();
    if ( idx == ((uint32_t)-1) )
        panic("alloc_frame(): No free frames!");

    // This page is ours.     
    set_frame(idx * 0x1000); 
    page->present = 1;   
    page->rw      = (is_writeable?1:0);
    page->user    = (is_kernel?0:1);
    page->frame   = idx; 
}

/*
 * free_frame deallocates frame.
 */
void free_frame(page_t *page)
{
    uint32_t frame = page->frame;
    if (frame == 0)
        return; // The given page doesn't actually have an allocated frame.

    clear_frame(frame);
    page->frame = 0x0;
}

/*
 * pager_get_page_impl
 */
static uint32_t pager_get_page_impl(pager_t *pager, uint32_t address, int32_t make)
{
    page_directory_t *dir = (page_directory_t *)pager->page_directory;
    return (uint32_t)get_page(address, make, dir);
}

/*
 * pager_alloc_frame_impl
 */
static void pager_alloc_frame_impl(pager_t *pager, uint32_t page)
{
    int32_t is_kernel    = pager->supervisor?1:0;
    int32_t is_writeable = pager->readonly?0:1;
    alloc_frame((page_t *)page, is_kernel, is_writeable);
}

/*
 * page_free_frame_impl
 */
static void pager_free_frame_impl(pager_t *pager, uint32_t page)
{
    return free_frame((page_t *)page);
}

/*
 * pager_build_physical_address_impl
 */
static uint32_t pager_build_physical_address_impl(pager_t *pager, uint32_t page, uint32_t address)
{
    page_t *p = (page_t *)page; 
    return ((p->frame * 0x1000) + (address & 0xFFF));
}

/*
 * pager_impl_export
 */
static void pager_impl_export(pager_t *pager, page_directory_t *dir, int32_t supervisor, int32_t readonly)
{
    pager->page_directory         = (uint32_t)dir;                               
    pager->supervisor             = supervisor;                                  
    pager->readonly               = readonly;      
    pager->get_page               = pager_get_page_impl;
    pager->alloc_frame            = pager_alloc_frame_impl;
    pager->free_frame             = pager_free_frame_impl;
    pager->build_physical_address = pager_build_physical_address_impl; 
}

/*
 * clone_table
 */
static page_table_t *clone_table(page_table_t *src, uint32_t *physAddr)
{
    ASSERT(sizeof(page_table_t) == 0x1000); 
    page_table_t *table = (page_table_t *)kmalloc_ap(sizeof(page_table_t), physAddr);
    memset((uint8_t *)table, 0, sizeof(page_table_t));  

    int32_t fa;
    for (fa = 0; fa < 1024; ++fa) 
    { 
        if (src->pages[fa].frame == 0)
            continue;

        // Get a new frame.
        alloc_frame(&table->pages[fa], 0, 0);
        // Copy the flags from source to destination. 
        table->pages[fa].present  = src->pages[fa].present;     
        table->pages[fa].rw       = src->pages[fa].rw;
        table->pages[fa].user     = src->pages[fa].user;
        table->pages[fa].accessed = src->pages[fa].accessed;
        table->pages[fa].dirty    = src->pages[fa].dirty;
        // Copy page data. This function is in copy_page_data. 
        // monitor_write ( "Outside copy page data! src: " ) ;   
        // monitor_write_hex ( src->pages[fa].frame*0x1000 ) ; 
        // monitor_write ( " dst: " ) ; monitor_write_hex ( table->pages[fa].frame*0x1000 ) ;
        // monitor_write ( "\n" ) ;
        copy_page_data(src->pages[fa].frame*0x1000, table->pages[fa].frame*0x1000);
    } 
    return table;
} 
