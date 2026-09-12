/* ============================================================================
 * NetrdorvOS - 内存管理 (阶段2：页分配、堆内存、内存回收)
 * ============================================================================ */

#ifndef __INCLUDE_KERNEL_MM_H__
#define __INCLUDE_KERNEL_MM_H__

#include <types.h>
#include <arch/mmu.h>

/* ---------- 物理页管理 ---------- */
#define PAGE_FREE       0
#define PAGE_USED       1

typedef struct page {
    struct page *next;          /* 空闲链表指针 */
    u32 refcount;
    u32 flags;
} page_t;

/* 初始化物理内存管理（阶段2，收集可用物理内存） */
void pmm_init(u64 mem_start, u64 mem_end);

/* 分配/释放物理页 */
void *alloc_pages(u32 count);
void free_pages(void *addr, u32 count);

/* ---------- 堆内存 (kalloc) ---------- */
void kheap_init(void *start, size_t size);
void *kmalloc(size_t size);
void kfree(void *ptr);

#endif /* __INCLUDE_KERNEL_MM_H__ */
