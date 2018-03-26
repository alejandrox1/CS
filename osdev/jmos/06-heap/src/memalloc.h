#ifndef MEMALLOC_H
#define MEMALLOC_H

#include "common.h"

// kmalloc
uint32_t kmalloc(uint32_t sz);
// kmalloc_a allocates page aligned chunks.
uint32_t kmalloc_a(uint32_t sz);
// kmalloc_p returns physical address.
uint32_t kmalloc_p(uint32_t sz, uint32_t *phys);
// kmalloc_ap page aligned and returns the physical address.
uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys);

#endif
