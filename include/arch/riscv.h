/* ============================================================================
 * NetrdorvOS - RISC-V CSR 操作与寄存器定义 (S-mode)
 * ============================================================================ */

#ifndef __INCLUDE_ARCH_RISCV_H__
#define __INCLUDE_ARCH_RISCV_H__

#include <types.h>

/* ---------- CSR 读写 ---------- */
#define csr_read(csr) ({ \
    u64 __v; \
    asm volatile("csrr %0, " #csr : "=r"(__v)); \
    __v; \
})

#define csr_write(csr, val) ({ \
    u64 __v = (u64)(val); \
    asm volatile("csrw " #csr ", %0" :: "r"(__v)); \
})

#define csr_set(csr, bits) ({ \
    u64 __v = (u64)(bits); \
    asm volatile("csrs " #csr ", %0" :: "r"(__v)); \
})

#define csr_clear(csr, bits) ({ \
    u64 __v = (u64)(bits); \
    asm volatile("csrc " #csr ", %0" :: "r"(__v)); \
})

/* ---------- 特权级 ---------- */
#define PRV_M   3
#define PRV_S   1
#define PRV_U   0

/* ---------- sstatus 位 ---------- */
#define SSTATUS_SIE     BIT(1)
#define SSTATUS_SPIE    BIT(5)
#define SSTATUS_SPP     BIT(8)
#define SSTATUS_FS      (3UL << 13)
#define SSTATUS_SUM     BIT(18)   /* S 态可访问用户页 */

/* ---------- sie/sip 位 ---------- */
#define IRQ_S_SOFT      1
#define IRQ_S_TIMER     5
#define IRQ_S_EXT       9

#define SIE_SSIE        BIT(IRQ_S_SOFT)
#define SIE_STIE        BIT(IRQ_S_TIMER)
#define SIE_SEIE        BIT(IRQ_S_EXT)

/* ---------- scause 中断位 ---------- */
#define SCAUSE_INTR     (1UL << 63)

/* ---------- 异常码 ---------- */
#define EXC_INST_MISALIGNED     0
#define EXC_INST_ACCESS         1
#define EXC_INST_ILLEGAL        2
#define EXC_BREAKPOINT          3
#define EXC_LOAD_MISALIGNED     4
#define EXC_LOAD_ACCESS         5
#define EXC_STORE_MISALIGNED    6
#define EXC_STORE_ACCESS        7
#define EXC_ECALL_U             8
#define EXC_ECALL_S             9
#define EXC_INST_PAGE_FAULT     12
#define EXC_LOAD_PAGE_FAULT     13
#define EXC_STORE_PAGE_FAULT    15

/* ---------- satp 模式 (Sv39) ---------- */
#define SATP_MODE_SV39          (8UL << 60)
#define SATP_PPN_MASK           0x00000FFFFFFFFFFFUL

/* ---------- 辅助 ---------- */
static inline u64 riscv_hartid(void)
{
    /* S 态读 mhartid 需 SBI 或 SATP 转发；Limine 传 a1 */
    return csr_read(sscratch);  /* boot.S 暂存 */
}

static inline void enter_user_mode(u64 entry, u64 user_sp)
{
    u64 sstat = csr_read(sstatus);
    sstat &= ~SSTATUS_SPP;      /* 清除 SPP → 返回 U 态 */
    csr_write(sstatus, sstat);
    csr_write(sepc, entry);
    csr_write(sscratch, user_sp);
    asm volatile("sret" ::: "memory");
}

#endif /* __INCLUDE_ARCH_RISCV_H__ */
