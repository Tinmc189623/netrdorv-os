/* ============================================================================
 * NetrdorvOS - S-mode 中断/异常处理
 * ecall → syscall 分发；定时器 → 调度 tick
 * ============================================================================ */

#ifndef __INCLUDE_ARCH_TRAP_H__
#define __INCLUDE_ARCH_TRAP_H__

#include <types.h>

/* 与 boot.S s_trap_vector 保存的寄存器布局对应 */
typedef struct {
    u64 ra;
    u64 gp;
    u64 tp;
    u64 t0, t1, t2;
    u64 s0, s1;
    u64 a0, a1, a2, a3, a4, a5, a6, a7;
    u64 s2, s3, s4, s5, s6, s7, s8, s9, s10, s11;
    u64 t3, t4, t5, t6;
    u64 sepc;
    u64 scause;
    u64 stval;
} trap_frame_t;

/* S-mode 异常统一入口（boot.S s_trap_vector 调用） */
void s_trap_handler(u64 sepc, u64 scause, u64 stval, trap_frame_t *frame);

/* 初始化 stvec 并使能 S 态中断 */
void trap_init(void);

#endif /* __INCLUDE_ARCH_TRAP_H__ */
