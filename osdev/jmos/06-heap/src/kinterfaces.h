#ifndef KINTERFACES_H
#define KINTERFACES_H

#include "common.h"

// Paging abstraction.
typedef struct pager_struct pager_t;

typedef uint32_t (*pager_get_page)               (pager_t *pager, uint32_t address, int32_t make);
typedef void     (*pager_alloc_frame)            (pager_t *pager, uint32_t page);
typedef void     (*page_free_frame)              (pager_t *pager, uint32_t page);
typedef uint32_t (*pager_build_physical_address) (pager_t *pager, uint32_t page, uint32_t address);

struct pager_struct
{
    uint32_t                     page_directory;
    int32_t                      supervisor;
    int32_t                      readonly;
    pager_get_page               get_page;
    pager_alloc_frame            alloc_frame;
    pager_free_frame             free_frame;
    pager_build_physical_address build_physical_address;
};


// Free space index abstractions.
typedef struct free_space_indexer_struct free_space_indexer_t;

typedef void    (*free_space_indexer_initialize)           (free_space_indexer_t *index);
typedef int32_t (*free_space_indexer_size_search_min2)     (free_space_indexer_t *index,
        uint32_t size, uint32_t *left, uint32_t *right, uint32_t *id);

typedef int32_t (*free_space_indexer_size_add)              (free_space_indexer_t *index,
        uint32_t left, uint32_t right, uint32_t *id);

typedef int32_t (*free_space_indexer_size_del)              (free_space_indexer_t *index,
        uint32_t id);

typedef int32_t (*free_space_indexer_left_size_search)      (free_space_indexer_t *index,
        uint32_t left, uint32_t *right, uint32_t *id);

typedef int32_t (*free_space_indexer_left_size_search_left) (free_space_indexer_t *index,
        uint32_t *left, uint32_t right, uint32_t *id);

typedef int32_t (*free_space_indexer_left_size_add)         (free_space_indexer_t *index,
        uint32_t id);

typedef int32_t (*free_space_indexer_left_size_del)         (free_space_indexer_t *index,
        uint32_t left);

typedef int32_t (*free_space_indexer_left_void_search)      (free_space_indexer_t *index,
        uint32_t left, uint32_t *right);

typedef int32_t (*free_space_indexer_left_void_add)         (free_space_indexer_t *index,
        uint32_t left, uint32_t right);

typedef int32_t (*free_space_indexer_left_void_del)         (free_space_indexer_t *index,
        uint32_t left);

struct free_space_indexer_struct
{
    uint32_t                                 address_begin;
    uint32_t                                 address_end;
    free_space_indexer_initialize            initialize;
    free_space_indexer_size_search_min2      size_search_min2;
    free_space_indexer_size_add              size_add;
    free_space_indexer_size_del              size_del;
    free_space_indexer_left_size_search      left_size_search;
    free_space_indexer_left_size_search_left left_size_search_left;
    free_space_indexer_left_size_add         left_size_add;
    free_space_indexer_left_size_del         left_size_del;
    free_space_indexer_left_void_search      left_void_search;
    free_space_indexer_left_void_add         left_void_add;
    free_space_indexer_left_void_del         left_void_del;
};
#endif // KINTERFACES_H
