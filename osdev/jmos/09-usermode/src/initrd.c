/*
 * initrd.c
 *
 * Implementation of our initial ramdisk API.
 *
 * Taken from JamesM's kernel development tutorials.
 */
#include "initrd.h"
#include "common.h"
#include "kheap.h"


initrd_header_t      *initrd_header;    // The header.
initrd_file_header_t *file_headers;     // List of file headers.
fs_node_t            *initrd_root;      // Root directory node.
fs_node_t            *initrd_dev;       // Directory node for dev - to mount devfs.
fs_node_t            *root_nodes;       // List of file nodes.
int                   nroot_nodes;      // Number of file nodes.

dirent_t diren;

static uint32_t   initrd_read   (fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
static dirent_t  *initrd_readdir(fs_node_t *node, uint32_t index);
static fs_node_t *initrd_finddir(fs_node_t *node, char *name);
/******************************************************************************
 *                                  Public API                                *
 *****************************************************************************/
/*
 *
 */
fs_node_t *initialize_initrd(uint32_t location)
{
    // Initialize the main and file header pointers and populate the root
    // directory.
    initrd_header = (initrd_header_t *)location;
    file_headers  = (initrd_file_header_t *)(location+sizeof(initrd_header_t));

    // Initialize the root directory.
    initrd_root = (fs_node_t *)kmalloc(sizeof(fs_node_t));
    strcpy(initrd_root->name, "initrd");
    initrd_root->mask = initrd_root->uid = initrd_root->gid = initrd_root->inode = initrd_root->length = 0;
    initrd_root->flags   = FS_DIRECTORY;
    initrd_root->read    = 0;
    initrd_root->write   = 0;
    initrd_root->open    = 0;
    initrd_root->close   = 0;
    initrd_root->readdir = &initrd_readdir;
    initrd_root->finddir = &initrd_finddir;
    initrd_root->ptr     = 0;
    initrd_root->impl    = 0;

    // Initialize the /dev directory.
    initrd_dev = (fs_node_t *)kmalloc(sizeof(fs_node_t));
    strcpy(initrd_dev->name, "dev");
    initrd_dev->mask = initrd_dev->uid = initrd_dev->gid = initrd_dev->inode = initrd_dev->length = 0;
    initrd_dev->flags   = FS_DIRECTORY;
    initrd_dev->read    = 0;
    initrd_dev->write   = 0;
    initrd_dev->open    = 0;
    initrd_dev->close   = 0;
    initrd_dev->readdir = &initrd_readdir;
    initrd_dev->finddir = &initrd_finddir;
    initrd_dev->ptr     = 0;
    initrd_dev->impl    = 0;

    // Allocate space for files.
    root_nodes  = (fs_node_t *)kmalloc(sizeof(fs_node_t *) * initrd_header->nfiles);
    nroot_nodes = initrd_header->nfiles;

    // For every file...
    int i;
    for (i = 0; i < initrd_header->nfiles; i++)
    {
        // Edit the file's header; it currently holds the file offset relative
        // to the start of ramdisk, we want it relative to the start of memory.
        file_headers[i].offset += location;

        // Create a new file node.
        strcpy(root_nodes[i].name, (const char *)&file_headers[i].name);
        root_nodes[i].mask = root_nodes[i].uid = root_nodes[i].gid = 0;
        root_nodes[i].length  = file_headers[i].length;
        root_nodes[i].inode   = i;
        root_nodes[i].flags   = FS_FILE;
        root_nodes[i].read    = &initrd_read;
        root_nodes[i].write   = 0;
        root_nodes[i].open    = 0;
        root_nodes[i].close   = 0;
        root_nodes[i].readdir = 0;
        root_nodes[i].finddir = 0;
        root_nodes[i].impl    = 0;
    }
    return initrd_root;
}


/******************************************************************************
 *                                 Private API                                *
 *****************************************************************************/
/*
 * initrd_read read from a file in our initrd.
 */
static uint32_t initrd_read(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer)
{
    initrd_file_header_t header = file_headers[node->inode];
    
    if (offset > header.length)
        return 0;
    if (offset+size > header.length)
        size = header.length - offset;
    
    memcpy(buffer, (uint8_t *)(header.offset + offset), size);
    return size;
}

/*
 * initrd_readdir
 */
static dirent_t *initrd_readdir(fs_node_t *node, uint32_t index)
{
    if (node == initrd_root && index == 0)
    {
        strcpy(diren.name, "dev");
        diren.name[3] = 0;
        diren.ino     = 0;
        return &diren;
    }

    if (index-1 >= nroot_nodes)
        return 0;

    strcpy(diren.name, root_nodes[index-1].name);
    diren.name[strlen(root_nodes[index-1].name)] = 0;
    diren.ino = root_nodes[index-1].inode;
    return &diren;
}

/*
 * initrd_finddir
 */
static fs_node_t *initrd_finddir(fs_node_t *node, char *name)
{
    if (node == initrd_root && !strcmp(name, "dev"))
        return initrd_dev;

    int i;
    for (i = 0; i < nroot_nodes; i++)
        if (!strcmp(name, root_nodes[i].name))
            return &root_nodes[i];
    return 0;
}
