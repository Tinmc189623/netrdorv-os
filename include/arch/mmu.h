/* ============================================================================
 * NetrdorvOS - MMU 管理 (Sv39 分页)
 * 阶段1：MMU 虚拟内存开启 (Sv39)
 * ============================================================================ */

#ifndef __INCLUDE_ARCH_MMU_H__
#define __INCLUDE_ARCH_MMU_H__

#include <types.h>

/* ---------- Sv39 分页参数 ---------- */
#define PAGE_SIZE       4096
#define PAGE_SHIFT      12
#define PTE_COUNT       512         /* 每级页表 512 项 */

/* ---------- PTE 标志位 ---------- */
#define PTE_V           BIT(0)      /* 有效 */
#define PTE_R           BIT(1)      /* 可读 */
#define PTE_W           BIT(2)      /* 可写 */
#define PTE_X           BIT(3)      /* 可执行 */
#define PTE_U           BIT(4)      /* 用户态可访问 */
#define PTE_G           BIT(5)      /* 全局映射 */
#define PTE_A           BIT(6)      /* 已访问 */
#define PTE_D           BIT(7)      /* 已脏 */

#define PTE_PPN_SHIFT   10
#define PTE_PPN_MASK    (((1UL << 44) - 1) << PTE_PPN_SHIFT)

/* ---------- Sv39 地址布局 ---------- */
/* 虚拟地址 39 位：VPN[2]:VPN[1]:VPN[0]:offset (9:9:9:12) */
#define VA_BITS         39
#define VPN_SHIFT_0     12
#define VPN_SHIFT_1     21
#define VPN_SHIFT_2     30

/* ---------- satp 寄存器 ---------- */
#define SATP_MODE_SV39  8UL
#define SATP_MODE_MASK  (0xFUL << 60)
#define SATP_ASID_SHIFT 44
#define SATP_PPN_SHIFT  0

#define satp_make(ppn, asid) \
    ((SATP_MODE_SV39 << 60) | ((u64)(asid) << SATP_ASID_SHIFT) | (ppn))

/* 物理地址 → 页帧号 */
#define pa_to_ppn(pa)   ((u64)(pa) >> PAGE_SHIFT)
#define ppn_to_pa(ppn)  ((u64)(ppn) << PAGE_SHIFT)

/* ---------- 接口 ---------- */

/* 初始化 Sv39 分页（建立内核直接映射/恒等映射） */
void mmu_init(void);

/* 切换地址空间 */
void mmu_switch(u64 satp);

/* 分配一个页表页（阶段2 接入物理页分配器） */
void *mmu_alloc_page(void);

/* 建立 4KB 页映射 */
int mmu_map_page(u64 *root_pt, u64 va, u64 pa, u64 flags);

#endif /* __INCLUDE_ARCH_MMU_H__ */
