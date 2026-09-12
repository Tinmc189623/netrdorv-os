/* ============================================================================
 * NetrdorvOS - ELF64 用户程序加载器
 * 解析 ELF 头，加载段到用户地址空间，设置用户栈
 * ============================================================================ */

#ifndef __INCLUDE_KERNEL_ELF_H__
#define __INCLUDE_KERNEL_ELF_H__

#include <types.h>

/* 加载用户 ELF，返回入口地址；失败返回 0 */
u64 elf_load_user(const void *image, size_t size, u64 *entry_out,
                  u64 *user_sp_out);

#endif /* __INCLUDE_KERNEL_ELF_H__ */
