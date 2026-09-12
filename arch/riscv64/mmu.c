/* ============================================================================
 * NetrdorvOS - MMU 管理 (Sv39 恒等映射)
 * 阶段1/2：建立内核页表并启用 MMU
 *
 * 设计：Sv39，三级页表，将整个内核物理地址区间恒等映射
 * （va == pa），权限为内核读写执行。
 * ============================================================================ */

#include <arch/mmu.h>
#include <arch/riscv.h>
#include <os.h>
#include <driver/uart.h>
#include <lib/string.h>

/* 三级页表，每级 512 项 */
static u64 root_pt[PTE_COUNT]  __attribute__((aligned(PAGE_SIZE)));
static u64 mid_pt0[PTE_COUNT]  __attribute__((aligned(PAGE_SIZE)));
static u64 mid_pt1[PTE_COUNT]  __attribute__((aligned(PAGE_SIZE)));

/* 恒等映射范围：0x80000000 ~ 0x90000000 (256MB) */
#define MAP_START   0x80000000UL
#define MAP_END     0x90000000UL

static inline u64 make_pte(u64 ppn, u64 flags)
{
    return (ppn << PTE_PPN_SHIFT) | flags;
}

void mmu_init(void)
{
    memset(root_pt, 0, sizeof(root_pt));
    memset(mid_pt0, 0, sizeof(mid_pt0));
    memset(mid_pt1, 0, sizeof(mid_pt1));

    /* 恒等映射：2MB 大页（Sv39 的一级映射）
     * 为简化，使用 4KB 标准页遍历更清晰。
     * 这里直接构造三级页表项。 */
    u64 pa = MAP_START;
    while (pa < MAP_END) {
        u64 vpn2 = (pa >> VPN_SHIFT_2) & 0x1FF;
        u64 vpn1 = (pa >> VPN_SHIFT_1) & 0x1FF;
        u64 vpn0 = (pa >> VPN_SHIFT_0) & 0x1FF;

        /* 选中间页表 */
        u64 *mid = (vpn2 < 256) ? mid_pt0 : mid_pt1;

        /* root → mid */
        if (!root_pt[vpn2]) {
            u64 mid_pa = (vpn2 < 256) ? (u64)mid_pt0 : (u64)mid_pt1;
            root_pt[vpn2] = make_pte(pa_to_ppn(mid_pa),
                                     PTE_V | PTE_R | PTE_W | PTE_X);
        }

        /* mid → （简化：直接用 2MB 大页，跳过第三级） */
        /* Sv39 中 PTE 同时设置 R/W/X 表示块映射（大页） */
        mid[vpn1] = make_pte(pa_to_ppn(pa & ~((1UL << 21) - 1)),
                             PTE_V | PTE_R | PTE_W | PTE_X);
        pa += (1UL << 21);   /* 每次 2MB */
        (void)vpn0;
    }

    /* 启用 MMU：写 satp */
    u64 ppn = pa_to_ppn((u64)root_pt);
    u64 satp = satp_make(ppn, 0);
    csr_write(satp, satp);
    asm volatile("sfence.vma zero, zero" ::: "memory");

    kprintf("[mmu] Sv39 enabled, satp=0x%lx, map 0x%lx-0x%lx\n",
            (unsigned long)csr_read(satp),
            (unsigned long)MAP_START, (unsigned long)MAP_END);
}

void *mmu_alloc_page(void)
{
    /* 阶段2 接入页分配器 */
    extern void *alloc_pages(u32 count);
    return alloc_pages(1);
}

int mmu_map_page(u64 *root_pt_ptr, u64 va, u64 pa, u64 flags)
{
    (void)root_pt_ptr; (void)va; (void)pa; (void)flags;
    /* TODO: 按需补全 4KB 页映射遍历 */
    return 0;
}
