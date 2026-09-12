/* ============================================================================
 * NetrdorvOS - VFS 实现
 * 设备节点表 + 文件描述符表
 * ============================================================================ */

#include <kernel/vfs.h>
#include <kernel/mm.h>
#include <lib/string.h>
#include <driver/uart.h>
#define MAX_DEVICES  16
#define MAX_FDS      32

static device_t devices[MAX_DEVICES];
static int       device_count;

/* 已打开文件描述符 */
typedef struct {
    device_t *dev;
    int       used;
    int       flags;
} fd_entry_t;

static fd_entry_t fd_table[MAX_FDS];

void vfs_init(void)
{
    device_count = 0;
    memset(fd_table, 0, sizeof(fd_table));
    kprintf("[vfs] initialized, %d device slots\n", MAX_DEVICES);
}

int vfs_register_device(const char *name, file_ops_t *ops, void *priv)
{
    if (device_count >= MAX_DEVICES)
        return -1;
    device_t *d = &devices[device_count++];
    strncpy(d->name, name, sizeof(d->name) - 1);
    d->ops = *ops;
    d->priv = priv;
    kprintf("[vfs] registered /dev/%s\n", name);
    return 0;
}

static device_t *find_device(const char *path)
{
    /* path 形如 /dev/fb0 */
    if (strncmp(path, "/dev/", 5) != 0)
        return NULL;
    const char *name = path + 5;
    for (int i = 0; i < device_count; i++)
        if (strcmp(devices[i].name, name) == 0)
            return &devices[i];
    return NULL;
}

int sys_open(const char *path, int flags)
{
    device_t *d = find_device(path);
    if (!d)
        return -1;
    for (int i = 0; i < MAX_FDS; i++) {
        if (!fd_table[i].used) {
            fd_table[i].used = 1;
            fd_table[i].dev = d;
            fd_table[i].flags = flags;
            if (d->ops.open)
                d->ops.open(path, flags);
            return i;
        }
    }
    return -1;
}

int sys_close(int fd)
{
    if (fd < 0 || fd >= MAX_FDS || !fd_table[fd].used)
        return -1;
    device_t *d = fd_table[fd].dev;
    if (d && d->ops.close)
        d->ops.close(fd);
    fd_table[fd].used = 0;
    return 0;
}

int sys_read(int fd, void *buf, size_t count)
{
    if (fd < 0 || fd >= MAX_FDS || !fd_table[fd].used)
        return -1;
    device_t *d = fd_table[fd].dev;
    if (d && d->ops.read)
        return d->ops.read(fd, buf, count);
    return 0;
}

int sys_write(int fd, const void *buf, size_t count)
{
    if (fd < 0 || fd >= MAX_FDS || !fd_table[fd].used)
        return -1;
    device_t *d = fd_table[fd].dev;
    if (d && d->ops.write)
        return d->ops.write(fd, buf, count);
    return 0;
}

int sys_ioctl(int fd, unsigned long request, void *arg)
{
    if (fd < 0 || fd >= MAX_FDS || !fd_table[fd].used)
        return -1;
    device_t *d = fd_table[fd].dev;
    if (d && d->ops.ioctl)
        return d->ops.ioctl(fd, request, arg);
    return 0;
}
