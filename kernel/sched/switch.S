/* ============================================================================
 * NetrdorvOS - 任务上下文切换 (RV64 汇编)
 * 阶段2：多任务调度
 *
 * RISC-V 调用约定：被调用者需保存 s0-s11、ra
 * 切换时保存 16 个寄存器到任务栈，切换栈后恢复新任务。
 * ============================================================================ */

    .section .text
    .align 2

/*
 * void sched_context_switch(u64 *old_context_sp, u64 new_context_sp);
 *   a0 = &old_task->context_sp  (保存当前 sp 的指针)
 *   a1 = new_task 的初始 sp (栈顶)
 *
 * 栈帧布局（切换时压入 16 个 64 位寄存器，共 128 字节）：
 *   +112  tp
 *   +104  gp
 *   +96   s11
 *   ...
 *   +8    s0
 *   +0    ra
 */
    .globl sched_context_switch
sched_context_switch:
    addi    sp, sp, -128
    sd      ra, 0(sp)
    sd      s0, 8(sp)
    sd      s1, 16(sp)
    sd      s2, 24(sp)
    sd      s3, 32(sp)
    sd      s4, 40(sp)
    sd      s5, 48(sp)
    sd      s6, 56(sp)
    sd      s7, 64(sp)
    sd      s8, 72(sp)
    sd      s9, 80(sp)
    sd      s10, 88(sp)
    sd      s11, 96(sp)
    sd      gp, 104(sp)
    sd      tp, 112(sp)

    sd      sp, 0(a0)           /* 保存旧 sp */

    mv      sp, a1              /* 切换到新栈 */

    ld      ra, 0(sp)
    ld      s0, 8(sp)
    ld      s1, 16(sp)
    ld      s2, 24(sp)
    ld      s3, 32(sp)
    ld      s4, 40(sp)
    ld      s5, 48(sp)
    ld      s6, 56(sp)
    ld      s7, 64(sp)
    ld      s8, 72(sp)
    ld      s9, 80(sp)
    ld      s10, 88(sp)
    ld      s11, 96(sp)
    ld      gp, 104(sp)
    ld      tp, 112(sp)
    addi    sp, sp, 128

    ret

/*
 * 新任务首次调度时，sched_context_switch 的 ret 跳到这里。
 * 此时 sp 已恢复到栈顶（context frame 之上）。
 * context frame 下方（sp-128 起）由 sched_create_task 预填：
 *   [sp-128] = entry 函数指针
 *   [sp-120] = arg
 * 本函数取出 entry/arg 并调用 entry(arg)。
 */
    .globl task_first_entry
task_first_entry:
    addi    sp, sp, -16
    sd      ra, 0(sp)
    ld      t0, -128(sp)        /* entry */
    ld      a0, -120(sp)        /* arg */
    jalr    ra, t0              /* 调用 entry(arg) */
    /* 任务返回后不应到达这里；若到达则死循环 */
1:  wfi
    j       1b
