/*
 * fs.h
 *
 * Define the API for a virtual filesystem.
 *
 * Taken fromJamesM's kernel development tutorials.
 */
#ifndef _FS_H
#define _FS_H

#include "types.h"

// fs_node_t - filesystem abstraction.
typedef struct fs_node_struct fs_node_t;
typedef struct dirent dirent_t;

// Define callbacks for VFS.
typedef uint32_t  (*read_type_t   )(fs_node_t*, uint32_t, uint32_t, uint8_t*);
typedef uint32_t  (*write_type_t  )(fs_node_t*, uint32_t, uint32_t, uint8_t*);
typedef void      (*open_type_t   )(fs_node_t*);
typedef void      (*close_type_t  )(fs_node_t*);
typedef dirent_t* (*readdir_type_t)(fs_node_t*, uint32_t);
typedef dirent_t* (*findir_type_t )(fs_node_t*, char*);


struct fs_node_struct
{
    char           name[128];
    uint32_t       mask;      // Permission mask.
    uint32_t       uid;       // Owning user.
    uint32_t       gid;       // Owning group.
    uint32_t       flags;     // I.e., node type (dir, file).
    uint32_t       inode;     // Device specific - a way for a filesystem to identify files.
    uint32_t       length;    // Size of file in bytes.
    uint32_t       impl;      // Implementation-defined number.
    read_type_t    read;
    write_type_t   write;
    open_type_t    open;
    close_type_t   close;
    readdir_type_t readdir;
    findir_type_t  finddir;
    fs_node_t      *ptr;      // Mountpoints and symlincs.
};


#endif // _FS_H
