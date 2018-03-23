#ifndef PAGING_H
#define PAGING_H

#include "common.h"
#include "idt.h"

/*
 * page_t
 */
typedef struct page
{
    uint32_t present  :  1;    // Page present in memory.
    uint32_t rw       :  1;    // Read-only if clear, readwrite is set.
    uint32_t user     :  1;    // Supervisor level only if clear.
    uint32_t accessed :  1;    // Has page been accessed since last refresh?
    uint32_t dirty    :  1;    // Has page been written to since last refresh?
    uint32_t unused   :  7;    // Amalgamation of unused and reserve bits.
    uint32_t frame    : 20;    // Frame address (shifted right 12 bits).
} __attribute__((packed)) page_t;


/*
 * page_table_t
 */
typedef struct page_table
{
    page_t pages[1024];
} __attribute__((packed)) page_table_t;


/*
 * page_directory_t
 */
typedef struct page_directory
{
    // Array of pointers to page tables.
    page_table_t *tables[1024];

    // Array of pointers to the page tables defined above. Get their "physical"
    // location for laoding into CR3 register.
    uint32_t tablesPhysical[1024];

    // Physical address of (tablesPhysical) the table of pointer to the page 
    // tables. This will be useful when we get out kernel heap allocated and
    // the directory may be in a different location in virtual memory.
    uint32_t physicalAddr;
} __attribute__((packed)) page_directory_t;





// initialize_paging sets up the environment, page directories, etc. It also
// enables paging.
void initialize_paging();

// switch_page_directory causes the specified page directory to be loaded into
// the CR3 register.
void switch_page_directory(page_directory_t *dir);

// get_page retrieves a pointer to the page required.
// If make == 1, if the page-table in which this page should reside is not 
// created, then create it.
page_t *get_page(uint32_t address, int32_t make, page_directory_t *dir);

// alloc_frame allocates a frame.                                               
void alloc_frame(page_t *page, int32_t is_kernel, int32_t is_writeable);
// free_frame deallocates frame.                                                                                                                                                                            
void free_frame(page_t *page);

void page_fault(registers_t *regs);

page_directory_t *clone_directory(page_directory_t *src);


#endif // PAGING_H
