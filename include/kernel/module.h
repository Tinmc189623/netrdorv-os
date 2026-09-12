/* ============================================================================
 * NetrdorvOS - 可加载内核模块 (.ko) 框架
 * 驱动以独立 .ko 文件存在，由模块加载器动态加载
 * 社区驱动只需实现 file_ops 并通过 module_init 注册
 * ============================================================================ */

#ifndef __INCLUDE_KERNEL_MODULE_H__
#define __INCLUDE_KERNEL_MODULE_H__

#include <types.h>
#include <kernel/vfs.h>

#define MODULE_NAME_LEN  32

typedef struct module {
    char        name[MODULE_NAME_LEN];
    int       (*init)(void);
    void      (*exit)(void);
    file_ops_t *ops;
    void       *priv;
    struct module *next;
} module_t;

/* 驱动模块注册：在 .ko 或内置驱动中定义此结构体并放入 .modules 段 */
#define MODULE_DEFINE(mod_name, init_fn, exit_fn, ops_ptr) \
    static module_t __mod_##mod_name = { \
        .name = #mod_name, \
        .init = (init_fn), \
        .exit = (exit_fn), \
        .ops = (ops_ptr), \
    }; \
    __attribute__((used, section(".modules"))) \
    static module_t *__modptr_##mod_name = &__mod_##mod_name;

/* 内核侧 API */
void module_init_all(void);
int  module_load(const char *path);
void module_unload(const char *name);

#endif /* __INCLUDE_KERNEL_MODULE_H__ */
