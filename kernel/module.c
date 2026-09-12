/* ============================================================================
 * NetrdorvOS - 模块加载器实现
 * 遍历 .modules 段中的模块指针，调用 init，注册设备
 * ============================================================================ */

#include <kernel/module.h>
#include <kernel/vfs.h>
#include <driver/uart.h>
#include <lib/string.h>

/* 链接脚本提供的 .modules 段边界 */
extern module_t *__modules_start[];
extern module_t *__modules_end[];

static module_t *module_list;

void module_init_all(void)
{
    int count = 0;
    for (module_t **p = __modules_start; p < __modules_end; p++) {
        module_t *m = *p;
        if (!m)
            continue;
        if (m->init)
            m->init();
        if (m->ops)
            vfs_register_device(m->name, m->ops, m->priv);
        count++;
    }
    kprintf("[module] initialized %d built-in modules\n", count);
}

int module_load(const char *path)
{
    (void)path;
    /* TODO: 从文件系统加载独立 .ko ELF，解析重定位，调用 init */
    kprintf("[module] load '%s' (TODO: filesystem-backed load)\n", path);
    return -1;
}

void module_unload(const char *name)
{
    (void)name;
}
