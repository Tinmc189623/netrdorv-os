/* ============================================================================
 * NetrdorvOS - 虚拟文件系统层
 * Unix 哲学：一切设备皆文件，open/read/write/ioctl/close
 * ============================================================================ */

#ifndef __INCLUDE_KERNEL_VFS_H__
#define __INCLUDE_KERNEL_VFS_H__

#include <types.h>

/* 文件操作接口（设备驱动/文件系统各自实现） */
typedef struct file_ops {
    int   (*open)(const char *path, int flags);
    int   (*read)(int fd, void *buf, size_t count);
    int   (*write)(int fd, const void *buf, size_t count);
    int   (*ioctl)(int fd, unsigned long request, void *arg);
    int   (*close)(int fd);
} file_ops_t;

/* 设备节点注册 */
typedef struct device {
    char        name[32];       /* /dev/xxx */
    file_ops_t  ops;
    void       *priv;
} device_t;

/* VFS 初始化（注册内置 /dev 节点） */
void vfs_init(void);

/* 注册设备驱动（由 .ko 模块调用） */
int vfs_register_device(const char *name, file_ops_t *ops, void *priv);

/* 系统调用实现 */
int   sys_open(const char *path, int flags);
int   sys_close(int fd);
int   sys_read(int fd, void *buf, size_t count);
int   sys_write(int fd, const void *buf, size_t count);
int   sys_ioctl(int fd, unsigned long request, void *arg);

#endif /* __INCLUDE_KERNEL_VFS_H__ */
