/*
 * initrd.h
 *
 * Defines an our API for an initial ramdisk.
 *
 * Taken from JamesM's kernel development tutorials.
 */
#ifndef _INITRD_H
#define _INITRD_H

#include "types.h"
#include "fs.h"

// initrd_header_t
typedef struct
{
    uint32_t nfiles;
} initrd_header_t;

// initrd_file_header_t
typedef struct
{
    uint8_t  magic;
    int8_t   name[64];
    uint32_t offset;
    uint32_t length;
} initrd_file_header_t;

// initialize_initrd initialises the initial ramdisk. 
// It takes the address of the multiboot module, and returns a completed 
// filesystem node.
fs_node_t *initialize_initrd(uint32_t location);

#endif // _INITRD_H
