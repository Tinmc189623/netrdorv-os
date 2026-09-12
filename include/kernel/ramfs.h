/* ============================================================================
 * NetrdorvOS - RAMFS 内存文件系统
 * 纯内存文件，支持 /home /root /apps /config /gui_res /usr/lib
 * ============================================================================ */

#ifndef __KERNEL_RAMFS_H__
#define __KERNEL_RAMFS_H__

#include <types.h>

#define RAMFS_NAME_LEN  64
#define RAMFS_MAX_FILES 64

typedef struct ramfs_file {
    char    name[RAMFS_NAME_LEN];
    u8     *data;
    size_t  size;
    size_t  cap;
    int     flags;
} ramfs_file_t;

void ramfs_init(void);
int  ramfs_create(const char *path, const void *data, size_t size);
int  ramfs_open(const char *path);
int  ramfs_read(int fd, void *buf, size_t n);
int  ramfs_write(int fd, const void *buf, size_t n);
int  ramfs_close(int fd);

#endif
