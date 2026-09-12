/* ============================================================================
 * NetrdorvOS - Limine 引导入口 (RV64 S-mode)
 *
 * 由 Limine 引导加载器进入 S-mode，hart 0 启动。
 * 流程：Limine 请求头 → 设置栈 → 清 BSS → 跳 kmain(C)
 * ============================================================================ */

    .section .limine_requests, "a"
    .balign 16

/* ---- 基础修订请求（必须第一个） ---- */
    .balign 16
    .quad  0xf956d6c8a7d61e71, 0x7c6167e50fad9b7f, 0xb124f5bb98aea3b6
    .quad  0                      /* revision */
    .quad  2                      /* base revision 2 */

/* ---- 内存映射请求 ---- */
    .balign 16
limine_memmap_req:
    .quad  0xc7b1dd30df4c8b88, 0x0a8e78244cc7f5ea
    .quad  0
    .quad  0                      /* response 由 Limine 填充 */

/* ---- 帧缓冲请求 ---- */
    .balign 16
limine_framebuffer_req:
    .quad  0x9d3a860e1b2e7e00, 0x0f7a45fd2db7d7ea
    .quad  0
    .quad  0

/* ---- HART ID 请求 ---- */
    .balign 16
limine_hartid_req:
    .quad  0x3b9ea37a61119e07, 0x356f8df3c721d5b9
    .quad  0
    .quad  0

/* ---- 内核地址请求 ---- */
    .balign 16
limine_kaddr_req:
    .quad  0x519e84ed1a4a5d66, 0x060c85d7b3a085dd
    .quad  0
    .quad  0

/* ---- 模块请求 ---- */
    .balign 16
limine_module_req:
    .quad  0x3e8dd40c0bd02c00, 0xf53ba5c1cf02fce9
    .quad  0
    .quad  0

/* ============================================================================
 * S-mode 入口
 * ============================================================================ */
    .section .text.entry
    .globl _start
_start:
    /* Limine 已在 S-mode，hart 0 */
    /* 设置内核栈（S-mode） */
    la      sp, __stack_top

    /* 清 BSS */
    la      t0, __bss_start
    la      t1, __bss_end
1:
    bgeu    t0, t1, 2f
    sd      zero, 0(t0)
    addi    t0, t0, 8
    j       1b
2:
    /* 保存 hartid（a1 由 Limine 传入） */
    mv      s0, a1

    /* 跳 C 入口 */
    call    kmain

1:
    wfi
    j       1b

/* ============================================================================
 * S-mode 中断/异常入口
 * ============================================================================ */
    .globl s_trap_vector
    .align 4
s_trap_vector:
    /* 保存上下文到内核栈 */
    addi    sp, sp, -256
    sd      ra, 0(sp)
    sd      gp, 8(sp)
    sd      tp, 16(sp)
    sd      t0, 24(sp)
    sd      t1, 32(sp)
    sd      t2, 40(sp)
    sd      s0, 48(sp)
    sd      s1, 56(sp)
    sd      a0, 64(sp)
    sd      a1, 72(sp)
    sd      a2, 80(sp)
    sd      a3, 88(sp)
    sd      a4, 96(sp)
    sd      a5, 104(sp)
    sd      a6, 112(sp)
    sd      a7, 120(sp)
    sd      s2, 128(sp)
    sd      s3, 136(sp)
    sd      s4, 144(sp)
    sd      s5, 152(sp)
    sd      s6, 160(sp)
    sd      s7, 168(sp)
    sd      s8, 176(sp)
    sd      s9, 184(sp)
    sd      s10, 192(sp)
    sd      s11, 200(sp)
    sd      t3, 208(sp)
    sd      t4, 216(sp)
    sd      t5, 224(sp)
    sd      t6, 232(sp)

    /* 读 scause/stval/sepc 调用 C 处理 */
    csrr    a0, sepc
    csrr    a1, scause
    csrr    a2, stval
    mv      a3, sp
    call    s_trap_handler

    /* 恢复 */
    ld      ra, 0(sp)
    ld      gp, 8(sp)
    ld      tp, 16(sp)
    ld      t0, 24(sp)
    ld      t1, 32(sp)
    ld      t2, 40(sp)
    ld      s0, 48(sp)
    ld      s1, 56(sp)
    ld      a0, 64(sp)
    ld      a1, 72(sp)
    ld      a2, 80(sp)
    ld      a3, 88(sp)
    ld      a4, 96(sp)
    ld      a5, 104(sp)
    ld      a6, 112(sp)
    ld      a7, 120(sp)
    ld      s2, 128(sp)
    ld      s3, 136(sp)
    ld      s4, 144(sp)
    ld      s5, 152(sp)
    ld      s6, 160(sp)
    ld      s7, 168(sp)
    ld      s8, 176(sp)
    ld      s9, 184(sp)
    ld      s10, 192(sp)
    ld      s11, 200(sp)
    ld      t3, 208(sp)
    ld      t4, 216(sp)
    ld      t5, 224(sp)
    ld      t6, 232(sp)
    addi    sp, sp, 256

    sret
