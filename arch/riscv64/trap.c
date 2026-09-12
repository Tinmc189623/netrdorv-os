/* ============================================================================
 * NetrdorvOS - S-mode 中断/异常处理
 * ecall → syscall 分发；stimer → 调度 tick
 * ============================================================================ */

#include <arch/trap.h>
#include <arch/riscv.h>
#include <arch/timer.h>
#include <kernel/syscall.h>
#include <kernel/sched.h>
#include <driver/uart.h>
#include <lib/string.h>

void s_trap_handler(u64 sepc, u64 scause, u64 stval, trap_frame_t *frame)
{
    (void)stval;

    if (scause & SCAUSE_INTR) {
        u64 irq = scause & ~SCAUSE_INTR;
        switch (irq) {
        case IRQ_S_TIMER:
            timer_set_next();
            sched_tick();
            break;
        case IRQ_S_SOFT:
            csr_clear(sip, SIE_SSIE);
            break;
        case IRQ_S_EXT:
            /* TODO: PLIC 外部中断分发 */
            break;
        default:
            kprintf("[trap] unknown S irq %lu\n", (unsigned long)irq);
            break;
        }
    } else {
        /* 异常 */
        switch (scause) {
        case EXC_ECALL_U:
            /* 用户态系统调用：a7 = 调用号 */
            frame->a0 = syscall_dispatch(frame->a7, frame->a0,
                                         frame->a1, frame->a2,
                                         frame->a3, frame->a4, frame->a5);
            /* 跳过 ecall 指令（4 字节） */
            sepc += 4;
            break;

        case EXC_ECALL_S:
            kprintf("[trap] ecall from S mode\n");
            break;

        default:
            kprintf("[trap] EXCEPTION sepc=0x%lx scause=0x%lx stval=0x%lx\n",
                    (unsigned long)sepc, (unsigned long)scause,
                    (unsigned long)stval);
            for (;;)
                ;
        }
    }

    /* 更新 sepc（ecall 跳过指令后） */
    if (scause == EXC_ECALL_U)
        csr_write(sepc, sepc);
}

void trap_init(void)
{
    extern char s_trap_vector[];
    csr_write(stvec, (u64)s_trap_vector);
    /* 使能 S 态定时器中断 */
    csr_set(sie, SIE_STIE);
    csr_set(sstatus, SSTATUS_SIE);
}
