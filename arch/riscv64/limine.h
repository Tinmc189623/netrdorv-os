/* ============================================================================
 * NetrdorvOS - Limine Boot Protocol 定义 (RISC-V64)
 * 参考：https://github.com/limine-bootloader/limine/blob/trunk/PROTOCOL.md
 * ============================================================================ */

#ifndef __ARCH_RISCV64_LIMINE_H__
#define __ARCH_RISCV64_LIMINE_H__

#include <types.h>

#define LIMINE_REQUEST_MAGIC0   0xf956d6c8a7d61e71ULL
#define LIMINE_REQUEST_MAGIC1   0x7c6167e50fad9b7fULL
#define LIMINE_REQUEST_MAGIC2   0xb124f5bb98aea3b6ULL

#define LIMINE_COMMON_MAGIC     0xc7b1dd30df4c8b88ULL, 0x0a8e78244cc7f5eaULL

/* 内存映射条目类型 */
#define LIMINE_MEMMAP_USABLE            0
#define LIMINE_MEMMAP_RESERVED          1
#define LIMINE_MEMMAP_ACPI_RECLAIM      2
#define LIMINE_MEMMAP_ACPI_NVS          3
#define LIMINE_MEMMAP_BAD               4
#define LIMINE_MEMMAP_BOOTLOADER_RECLAIM 5
#define LIMINE_MEMMAP_KERNEL_AND_MODULE 6
#define LIMINE_MEMMAP_FRAMEBUFFER       7

struct limine_uuid {
    u32 a;
    u16 b;
    u16 c;
    u8  d[8];
};

struct limine_memmap_entry {
    u64 base;
    u64 length;
    u64 type;
};

struct limine_memmap_response {
    u64 revision;
    u64 entry_count;
    struct limine_memmap_entry **entries;
};

struct limine_framebuffer {
    void *address;
    u64 width;
    u64 height;
    u64 pitch;
    u16 bpp;
    u8  memory_model;
    u8  red_mask_size;
    u8  red_mask_shift;
    u8  green_mask_size;
    u8  green_mask_shift;
    u8  blue_mask_size;
    u8  blue_mask_shift;
};

struct limine_framebuffer_response {
    u64 revision;
    u64 framebuffer_count;
    struct limine_framebuffer **framebuffers;
};

struct limine_hardware_offset_response {
    u64 revision;
    u64 hardware_offset;
};

struct limine_kernel_address_response {
    u64 revision;
    u64 physical_base;
    u64 virtual_base;
};

struct limine_hart_id_response {
    u64 revision;
    u64 id;
};

struct limine_module_response {
    u64 revision;
    u64 module_count;
    struct limine_module {
        u64  start;
        u64  end;
        char *cmdline;
    } **modules;
};

struct limine_term_response {
    u64 revision;
    u64 flags;
    void *term_write;
};

/* ---------- 请求结构体（放在 .limine_requests 段） ---------- */

#define LIMINE_REQUEST(name, magic, response_type) \
    struct { u64 id[3]; u64 revision; response_type *response; } name

/* 基础修订请求（必须第一个） */
struct limine_base_revision_request {
    u64 id[3];
    u64 revision;
    u64 base_revision;
};

#endif /* __ARCH_RISCV64_LIMINE_H__ */
