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

// fs_node_t.flags
#define FS_FILE        0x01                                                     
#define FS_DIRECTORY   0x02                                                     
#define FS_CHARDEVICE  0x03                                                     
#define FS_BLOCKDEVICE 0x04                                                     
#define FS_PIPE        0x05                                                     
#define FS_SYMLINK     0x06                                                     
#define FS_MOUNTPOINT  0x08 // Is the file an active mountpoint? 


// fs_node_t - filesystem abstraction.
typedef struct fs_node_struct fs_node_t;
typedef struct dirent dirent_t;

// Define callbacks for VFS.
typedef uint32_t   (*read_type_t   )(fs_node_t*, uint32_t, uint32_t, uint8_t*);
typedef uint32_t   (*write_type_t  )(fs_node_t*, uint32_t, uint32_t, uint8_t*);
typedef void       (*open_type_t   )(fs_node_t*);
typedef void       (*close_type_t  )(fs_node_t*);
typedef dirent_t*  (*readdir_type_t)(fs_node_t*, uint32_t);
typedef fs_node_t* (*finddir_type_t)(fs_node_t*, char*);

/*
 * fs_node_t
 */
struct fs_node_struct
{
    char            name[128];
    uint32_t        mask;      // Permission mask.
    uint32_t        uid;       // Owning user.
    uint32_t        gid;       // Owning group.
    uint32_t        flags;     // I.e., node type (dir, file).
    uint32_t        inode;     // Device specific - a way for a filesystem to identify files.
    uint32_t        length;    // Size of file in bytes.
    uint32_t        impl;      // Implementation-defined number.
    read_type_t     read;
    write_type_t    write;
    open_type_t     open;
    close_type_t    close;
    readdir_type_t  readdir;
    finddir_type_t  finddir;
    fs_node_t       *ptr;      // Mountpoints and symlincs.
};

/*
 * dirent_t
 */
struct dirent
{
    char     name[128];
    uint32_t ino;       // inode number.
};


// fs_root is the root of our filesystem.
extern fs_node_t *fs_root;

/*
 * Standard read/write/open/close operations for dealing with file nodes as
 * opposed to file descriptors.
 */
uint32_t   read_fs   (fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
uint32_t   write_fs  (fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
void       open_fs   (fs_node_t *node, uint8_t read, uint8_t write);
void       close_fs  (fs_node_t *node);
dirent_t  *readdir_fs(fs_node_t *node, uint32_t index);
fs_node_t *finddir_fs(fs_node_t *node, char *name);


#endif // _FS_H
