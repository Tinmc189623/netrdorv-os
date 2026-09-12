/* ============================================================================
 * NetrdorvOS - RAMFS 实现
 * ============================================================================ */

#include <kernel/ramfs.h>
#include <kernel/mm.h>
#include <lib/string.h>
#include <driver/uart.h>

static ramfs_file_t files[RAMFS_MAX_FILES];
static int file_count;

void ramfs_init(void)
{
    file_count = 0;
    kprintf("[ramfs] mounted, %d slots\n", RAMFS_MAX_FILES);
}

int ramfs_create(const char *path, const void *data, size_t size)
{
    if (file_count >= RAMFS_MAX_FILES) return -1;
    ramfs_file_t *f = &files[file_count++];
    strncpy(f->name, path, RAMFS_NAME_LEN - 1);
    f->data = kmalloc(size ? size : 1);
    f->size = size;
    f->cap = size;
    if (data && size) memcpy(f->data, data, size);
    f->flags = 0;
    return file_count - 1;
}

static ramfs_file_t *find_file(const char *path)
{
    for (int i = 0; i < file_count; i++)
        if (strcmp(files[i].name, path) == 0)
            return &files[i];
    return NULL;
}

int ramfs_open(const char *path)
{
    ramfs_file_t *f = find_file(path);
    return f ? (int)(f - files) : -1;
}

int ramfs_read(int fd, void *buf, size_t n)
{
    if (fd < 0 || fd >= file_count) return -1;
    ramfs_file_t *f = &files[fd];
    if (n > f->size) n = f->size;
    memcpy(buf, f->data, n);
    return (int)n;
}

int ramfs_write(int fd, const void *buf, size_t n)
{
    if (fd < 0 || fd >= file_count) return -1;
    ramfs_file_t *f = &files[fd];
    if (n > f->cap) {
        kfree(f->data);
        f->data = kmalloc(n);
        f->cap = n;
    }
    memcpy(f->data, buf, n);
    f->size = n;
    return (int)n;
}

int ramfs_close(int fd) { (void)fd; return 0; }
