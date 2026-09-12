/* ============================================================================
 * NetrdorvOS - 物理内存管理 + 堆分配器 (阶段2)
 *
 * 设计：
 *   1. 页分配器：位图法，管理一块预留物理内存区域
 *   2. 堆分配器：首次适配（first-fit）空闲链表
 *
 * 注意：实际物理内存大小需由设备树/启动协议告知。
 * 此处用编译期常量配置，真机上需按 DuoS DDR 大小调整。
 * ============================================================================ */

#include <kernel/mm.h>
#include <types.h>
#include <os.h>
#include <driver/uart.h>
#include <lib/string.h>

/* ---- 物理内存区域配置（TODO: 按 DuoS 实际 DDR 调整） ---- */
#define PMM_REGION_START   0x80400000UL   /* 内核镜像之上 */
#define PMM_REGION_SIZE    (32UL << 20)   /* 32MB 供页分配器 */

/* ---- 堆区域配置 ---- */
#define HEAP_START         0x82400000UL   /* 32MB 之后 */
#define HEAP_SIZE          (32UL << 20)   /* 32MB 堆 */

/* ---- 页分配器：位图 ---- */
#define PAGE_BITS  (sizeof(u64) * 8)
static u64 page_bitmap[(PMM_REGION_SIZE / PAGE_SIZE + PAGE_BITS - 1) / PAGE_BITS];
static u64 total_pages = PMM_REGION_SIZE / PAGE_SIZE;

/* ---- 堆分配器：空闲块链表 ---- */
typedef struct block_header {
    size_t size;                        /* 块大小（含头部） */
    int free;
    struct block_header *next;
} block_header_t;

static block_header_t *heap_head = NULL;

/* ---------------- 页分配 ---------------- */
static int page_is_free(u64 idx)
{
    return !(page_bitmap[idx / 64] & (1UL << (idx % 64)));
}

static void page_set(u64 idx, int used)
{
    if (used)
        page_bitmap[idx / 64] |= (1UL << (idx % 64));
    else
        page_bitmap[idx / 64] &= ~(1UL << (idx % 64));
}

void pmm_init(u64 mem_start, u64 mem_end)
{
    (void)mem_start; (void)mem_end;
    memset(page_bitmap, 0, sizeof(page_bitmap));
    kprintf("[pmm] region 0x%lx - 0x%lx (%lu pages, %lu MB)\n",
            (unsigned long)PMM_REGION_START,
            (unsigned long)(PMM_REGION_START + PMM_REGION_SIZE),
            (unsigned long)total_pages,
            (unsigned long)(PMM_REGION_SIZE >> 20));
}

void *alloc_pages(u32 count)
{
    if (count == 0)
        return NULL;
    /* 简单首次适配：找连续 count 个空闲页 */
    u64 run_start = 0, run_len = 0;
    for (u64 i = 0; i < total_pages; i++) {
        if (page_is_free(i)) {
            if (run_len == 0)
                run_start = i;
            run_len++;
            if (run_len == count) {
                for (u64 j = run_start; j < run_start + count; j++)
                    page_set(j, 1);
                return (void *)(PMM_REGION_START + run_start * PAGE_SIZE);
            }
        } else {
            run_len = 0;
        }
    }
    return NULL;
}

void free_pages(void *addr, u32 count)
{
    u64 idx = ((u64)addr - PMM_REGION_START) / PAGE_SIZE;
    for (u32 i = 0; i < count; i++)
        page_set(idx + i, 0);
}

/* ---------------- 堆分配（first-fit） ---------------- */
void kheap_init(void *start, size_t size)
{
    heap_head = (block_header_t *)start;
    heap_head->size = size;
    heap_head->free = 1;
    heap_head->next = NULL;
    kprintf("[heap] start=%p size=%lu KB\n", start, (unsigned long)(size >> 10));
}

static block_header_t *find_free(block_header_t **prev, size_t size)
{
    block_header_t *p = heap_head;
    block_header_t *prev_p = NULL;
    while (p) {
        if (p->free && p->size >= size + sizeof(block_header_t)) {
            *prev = prev_p;
            return p;
        }
        prev_p = p;
        p = p->next;
    }
    *prev = prev_p;
    return NULL;
}

void *kmalloc(size_t size)
{
    if (size == 0)
        return NULL;
    size = ALIGN_UP(size, 16);      /* 16 字节对齐 */

    block_header_t *prev = NULL;
    block_header_t *b = find_free(&prev, size);
    if (!b) {
        kprintf("[heap] OOM! requested %lu\n", (unsigned long)size);
        return NULL;
    }

    /* 若剩余空间足够再切一个块，则切分 */
    size_t remain = b->size - size;
    if (remain >= sizeof(block_header_t) + 16) {
        block_header_t *nb = (block_header_t *)((u8 *)b + size);
        nb->size = remain - size;
        nb->free = 1;
        nb->next = b->next;
        b->size = size;
        b->next = nb;
    }
    b->free = 0;
    return (void *)((u8 *)b + sizeof(block_header_t));
}

void kfree(void *ptr)
{
    if (!ptr)
        return;
    block_header_t *b = (block_header_t *)((u8 *)ptr - sizeof(block_header_t));
    b->free = 1;
    /* 与后向空闲块合并（简单版） */
    if (b->next && b->next->free) {
        b->size += b->next->size;
        b->next = b->next->next;
    }
}
