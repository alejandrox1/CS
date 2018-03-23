/*
 * kheap.c
 *
 * Taken from James' kernel development tutorials.
 */
#include "kheap.h"
#include "kinterfaces.h"
#include "paging.h"
#include "kassert.h"

typedef struct
{
    uint32_t             start_address;
    uint32_t             hole_start_address;
    uint32_t             end_address;
    uint32_t             max_address;
    free_space_indexer_t free_space_indexer;
    pager_t              pager;
} kheap_t;


// exertern
// Allow pagin put kheap_storage in a frame.
static kheap_t *kheap = 0;
static kheap_t kheap_storage;
// end is defined in the linker script.
extern uint32_t end;
// Used in paging.c
uint32_t placement_address = (uint32_t)&end;


////extern page_directory_t *kernel_directory;


int alloc(kheap_t *heap, uint32_t size, int32_t align, void **pp);     
void free(kheap_t *heap, void *p);
static int32_t expand(kheap_t *heap, uint32_t delta);
static void contract(kheap_t *heap, uint32_t delta);

/******************************************************************************
 *                                Public API                                  *
 *****************************************************************************/
/*
 * kmalloc_int
 */
uint32_t kmalloc_int(uint32_t sz, int32_t align, uint32_t *phys)
{
    if (kheap == 0)
    {
        if (align != 0)
        {
            if ((placement_address & 0xFFF) != 0)
            {
                placement_address -= (placement_address & 0xFFF);
                placement_address += 0x1000;
            }
        }
        ASSERT((align!=0)?((placement_address&0xFFF)==0):1);
        if (phys != 0)
            *phys = placement_address;
        uint32_t tmp = placement_address;
        placement_address += sz;
        return tmp;
    }

    void *p;
    int32_t status = alloc(kheap, sz, align, &p);
    ASSERT(status == 0);
    uint32_t address = (uint32_t)p;
    ASSERT((align!=0)?((address&0xFFF)==0):1);
    if (phys != 0)
    {
        // Callback impl decoupling.
        uint32_t page = kheap->pager.get_page(&kheap->pager, address, 0);
        *phys = kheap->pager.build_physical_address(&kheap->pager, page, address);
        ASSERT((align!=0)?(((*phys)&0xFFF)==0):1);
    }
    return address;
}

/*
 * kfree
 */
void kfree(uint32_t p)
{
    if (kheap != 0)
        free(kheap, (void *)p);
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
 * create_heap
 */
void create_heap(heap_params_t *heap_params)
{
    ASSERT(heap_params != 0);
    ASSERT(heap_params->heap_start_address > 0);
    ASSERT(heap_params->heap_start_address < heap_params->hole_start_address);
    ASSERT(heap_params->hole_start_address < heap_params->heap_end_address);
    ASSERT((heap_params->heap_end_address & 0xFFF) == 0);
    ASSERT(heap_params->heap_end_address < heap_params->heap_stop_address);
    ASSERT((heap_params->heap_stop_address & 0xFFF) == 0);

    // Set memory related attributes.
    kheap_t *hi = &kheap_storage;
    hi->start_address      = heap_params->heap_start_address;
    hi->hole_start_address = heap_params->hole_start_address;
    hi->end_address        = heap_params->heap_end_address;
    hi->max_address        = heap_params->heap_stop_address;

    // Set the free space params.
    hi->free_space_indexer               = heap_params->free_space_index;
    hi->free_space_indexer.address_begin = hi->start_address;     // Memory used by free_sapce_index
    hi->free_space_indexer.address_end   = hi->hole_start_address; //Bookeeping.
    hi->free_space_indexer.initialize(&hi->free_space_indexer);

    int32_t status;
    uint32_t id;
    free_space_indexer_t *x = &hi->free_space_indexer;
    status = x->size_add(x, hi->hole_start_address, hi->end_address, &id);
    ASSERT(status == 0);
    status = x->left_size_add(x, id);
    ASSERT(status == 0);

    // Set pager.
    hi->pager = heap_params->pager;
    // BOOM : kmalloc, kfree now both switch to using the heap                    
    // instead of placing alloc, respectively nothing                             
    kheap = hi ;
}

/*
 * alloc
 */
int32_t alloc(kheap_t *heap,uint32_t size, int32_t page_align, void **pp)
{
    uint32_t request_size = size;
    if (page_align != 0)
        request_size += 0x1000;

    // Find smallest hole.
    uint32_t left, right, id, id_left, id_right;
    free_space_indexer_t *x = &heap->free_space_indexer;
    int32_t status = x->size_search_min2(x, request_size, &left, &right, &id);
    if (status != 0)
    {
        // Need to make the heap larger.
        status = expand(heap, request_size);
        if (status != 0)
            return 1; 

        status = x->size_search_min2(x, request_size, &left, &right, &id);
        ASSERT(status == 0);
    }

    // Handle align, righ-1-left > size.
    uint32_t left_return = left, right_return = right;
    if (page_align != 0)
    {
        if ((left & 0xFFF) != 0)
        {
            ASSERT(left_return == left);
            left_return -= (left_return & 0xFFF);
            left_return += 0x1000;
        }
    }
    ASSERT(left <= left_return);
    ASSERT(left_return+size <= right+1);
    if (left_return+size < right+1)
        right_return = left_return + size - 1;
    ASSERT(right_return <= right);

    if (left < left_return) 
    { 
        if (right_return < right) 
        {  
            status = x->left_size_del(x, left);                        ASSERT(status == 0);
            status = x->size_del(x, id);                               ASSERT(status == 0);
            status = x->size_add(x, left, left_return-1, &id_left);    ASSERT(status == 0);
            status = x->size_add(x, right_return+1, right, &id_right); ASSERT(status == 0);
            status = x->left_size_add(x, id_left);                     ASSERT(status == 0);
            status = x->left_void_add(x, left_return, right_return);   ASSERT(status == 0);
            status = x->left_size_add(x, id_right);                    ASSERT(status == 0);
        } 
        else 
        {
            status = x->left_size_del(x, left);                        ASSERT(status == 0);
            status = x->size_del(x, id);                               ASSERT(status == 0);
            status = x->size_add(x, left, left_return-1, &id_left);    ASSERT(status == 0);
            status = x->left_void_add(x, left_return, right);          ASSERT(status == 0);
            status = x->left_size_add(x, id_left);                     ASSERT(status == 0);
        } 
    } 
    else 
    { 
        if (right_return < right) 
        {  
            status = x->left_size_del(x, left);                        ASSERT(status == 0);
            status = x->size_del(x, id);                               ASSERT(status == 0);
            status = x->size_add(x, right_return+1, right, &id_right); ASSERT(status == 0);
            status = x->left_void_add(x, left, right_return);          ASSERT(status == 0);
            status = x->left_size_add(x, id_right);                    ASSERT(status == 0);
        } 
        else 
        { 
            status = x->left_size_del(x, left);                        ASSERT(status == 0);
            status = x->size_del(x, id);                               ASSERT(status == 0);
            status = x->left_void_add(x, left, right);                 ASSERT(status == 0);
        }
    } 
    *pp = (void*)left_return; 
    return 0; 
}

/*
 * free
 */
void free(kheap_t *heap, void *p)
{
    uint32_t left = (uint32_t)p, 
             right, left_left, right_right, idleft, idright, idnew;
    free_space_indexer_t *x = &heap->free_space_indexer;
    int32_t status = x->left_void_search(x, left, &right);
    if ( status != 0 ) 
        return ; // p is not in our heap 

    int32_t unify_left  = x->left_size_search_left(x, &left_left, left-1, &idleft);
    int32_t unify_right = x->left_size_search(x, right+1, &right_right, &idright); 
    uint32_t aleft, aright;
    if ( unify_left == 0) 
    {   
        if ( unify_right == 0 ) 
        {    
            aleft  = left_left; 
            aright = right_right; 
            // left unify + right_unify                                               
            status = x->left_void_del(x, left);                       ASSERT(status == 0);
            status = x->left_size_del(x, left_left);                  ASSERT(status == 0);
            status = x->left_size_del(x, right+1);                    ASSERT(status == 0);
            status = x->size_del(x, idleft);                          ASSERT(status == 0);
            status = x->size_del(x, idright);                         ASSERT(status == 0);
            status = x->size_add(x, left_left, right_right, &idnew ); ASSERT(status == 0);
            status = x->left_size_add(x, idnew);                      ASSERT(status == 0);
        } 
        else 
        {  
            // left unify
            aleft  = left_left; 
            aright = right;   
            status = x->left_void_del(x, left);                       ASSERT(status == 0);
            status = x->left_size_del(x, left_left);                  ASSERT(status == 0);
            status = x->size_del(x, idleft);                          ASSERT(status == 0);
            status = x->size_add(x, left_left, right, &idnew);        ASSERT(status == 0);
            status = x->left_size_add(x, idnew);                      ASSERT(status == 0);
        }
    } 
    else 
    { 
        if ( unify_right == 0 ) 
        { 
            // right unify 
            aleft  = left; 
            aright = right_right;    
            status = x->left_void_del(x, left);                        ASSERT(status == 0);
            status = x->left_size_del(x, right+1);                     ASSERT(status == 0);
            status = x->size_del(x, idright);                          ASSERT(status == 0);
            status = x->size_add(x, left, right_right, &idnew);        ASSERT(status == 0);
            status = x->left_size_add(x, idnew);                       ASSERT(status == 0);
        } 
        else 
        { 
            // no unify at all   
            aleft  = left; 
            aright = right; 
            status = x->left_void_del(x, left);
            status = x->size_add(x, left, right, &idnew);              ASSERT(status == 0);
            status = x->left_size_add(x, idnew);                       ASSERT(status == 0);
        } 
    } 
    // Contract the heap if the new free space is adjacent to heap end.           
    if ( aright == heap->end_address ) 
        contract(heap, aright-aleft);
}
/******************************************************************************
 *                               Private API                                  *
 *****************************************************************************/
/*
 * expand
 */
static int32_t expand(kheap_t *heap, uint32_t delta)
{
    ASSERT((heap->end_address & 0xFFF) == 0);
    ASSERT(heap->start_address < heap->end_address);

    uint32_t new_end_address = heap->end_address + delta;

    // Page align new size.
    if ((new_end_address & 0xFFF) != 0)
    {
        new_end_address -= (new_end_address & 0xFFF);
        new_end_address += 0x1000;
    }
    ASSERT((new_end_address & 0xFFF) == 0);
    if (new_end_address > heap->max_address)
        return -1;

    uint32_t i = 0;
    for (i = heap->end_address; i < new_end_address; i += 0x1000)
    {
        // Use the callback impl decoupling.
        uint32_t page = heap->pager.get_page(&heap->pager, i, 1);
        heap->pager.alloc_frame(&heap->pager, page);
    }

    // Update end address.
    heap->end_address = new_end_address;
    return 0;
}

/*
 * contract
 */
static void contract(kheap_t *heap, uint32_t delta)
{
    ASSERT((heap->end_address & 0xFFF) == 0);
    ASSERT(heap->start_address < heap->end_address);

    uint32_t new_end_address = heap->end_address - delta;

    // page align new size.
    if ((new_end_address & 0xFFF) != 0)
    {
        new_end_address -= (new_end_address & 0xFFF);
        new_end_address += 0x1000;
    }
    ASSERT((new_end_address & 0xFFF) == 0);
    ASSERT(heap->hole_start_address <= new_end_address);
    ASSERT(new_end_address <= heap->max_address);

    uint32_t heap_old_end_address = heap->end_address;
    uint32_t fa = heap_old_end_address - 0x1000;
    uint32_t re = new_end_address;
    while (re < fa)
    {
        // Using the callback impl decoupling.
        uint32_t page = heap->pager.get_page(&heap->pager, fa, 0);
        heap->pager.free_frame(&heap->pager, page);
        fa -= 0x1000;
    }

    // Update end address.
    heap->end_address = new_end_address;
}
