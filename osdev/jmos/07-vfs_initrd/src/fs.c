/*
 * fs.c
 *
 * Implementation of filesystem API.
 *
 * Taken from JamesM's kernel development tutorials.
 */
#include "fs.h"

// Global
fs_node_t *fs_root = 0;

/******************************************************************************
 *                                 Public API                                 *
 *****************************************************************************/
/*
 * read_fs
 */
uint32_t read_fs(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer)
{
    // Does the node have a read callback?
    if (node->read)
        return node->read(node, offset, size, buffer);
    else
        return 0;
}

/*
 * write_fs
 */
uint32_t write_fs(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer)
{
    if (node->write)
        return node->write(node, offset, size, buffer);
    else
        return 0;
}

/*
 * open_fs
 */
void open_fs(fs_node_t *node, uint8_t read, uint8_t write)
{
    if (node->open)
        return node->open(node);
}

/*
 * close_fs
 */
void close_fs(fs_node_t *node)
{
    if (node->close)
        return node->close(node);
}

/*
 * readdir_fs
 */
dirent_t *readdir_fs(fs_node_t *node, uint32_t index)
{
    if ((node->flags&0x7) == FS_DIRECTORY && node->readdir != 0)
        return node->readdir(node, index);
    else
        return 0;
}

/*
 * finddir_fs
 */
fs_node_t *finddir_fs(fs_node_t *node, char *name)
{
    if ((node->flags&0x7) == FS_DIRECTORY && node->finddir != 0)
        return node->finddir(node, name);
    else
        return 0;
}
/******************************************************************************
 *                                 Private API                                *
 *****************************************************************************/
