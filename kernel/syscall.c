/* ============================================================================
 * NetrdorvOS - 系统调用分发表
 * ecall 从 U/S 态进入后，按 a7 号调用
 * ============================================================================ */

#include <kernel/syscall.h>
#include <kernel/vfs.h>
#include <kernel/sched.h>
#include <driver/uart.h>
#include <lib/string.h>

/* 系统调用表：参数在 a0~a5，返回值放 a0 */
typedef u64 (*syscall_fn_t)(u64, u64, u64, u64, u64, u64);

static u64 sys_exit(u64 code, u64 a1, u64 a2, u64 a3, u64 a4, u64 a5)
{
    (void)a1; (void)a2; (void)a3; (void)a4; (void)a5;
    kprintf("[syscall] exit(%lu)\n", (unsigned long)code);
    sched_halt();
    return 0;
}

static u64 sys_yield(u64 a0, u64 a1, u64 a2, u64 a3, u64 a4, u64 a5)
{
    (void)a0; (void)a1; (void)a2; (void)a3; (void)a4; (void)a5;
    sched_yield();
    return 0;
}

static u64 sys_sleep(u64 ticks, u64 a1, u64 a2, u64 a3, u64 a4, u64 a5)
{
    (void)a1; (void)a2; (void)a3; (void)a4; (void)a5;
    sched_sleep(ticks);
    return 0;
}

static u64 sys_getpid(u64 a0, u64 a1, u64 a2, u64 a3, u64 a4, u64 a5)
{
    (void)a0; (void)a1; (void)a2; (void)a3; (void)a4; (void)a5;
    return sched_current()->id;
}

static u64 sys_brk(u64 addr, u64 a1, u64 a2, u64 a3, u64 a4, u64 a5)
{
    (void)a1; (void)a2; (void)a3; (void)a4; (void)a5;
    /* 简化：直接返回当前堆顶 */
    return addr;
}

static syscall_fn_t syscall_table[SYS_MAX] = {
    [SYS_OPEN]        = (syscall_fn_t)sys_open,
    [SYS_CLOSE]       = (syscall_fn_t)sys_close,
    [SYS_READ]        = (syscall_fn_t)sys_read,
    [SYS_WRITE]       = (syscall_fn_t)sys_write,
    [SYS_IOCTL]       = (syscall_fn_t)sys_ioctl,
    [SYS_EXIT]        = sys_exit,
    [SYS_SCHED_YIELD] = sys_yield,
    [SYS_SLEEP]       = sys_sleep,
    [SYS_GETPID]      = sys_getpid,
    [SYS_FORK]        = 0,   /* TODO: 进程复制 */
    [SYS_EXECVE]      = 0,   /* TODO: ELF 加载 */
    [SYS_BRK]         = sys_brk,
    [SYS_MMAP]        = 0,
    [SYS_OPENAT]      = 0,
};

u64 syscall_dispatch(u64 number, u64 a0, u64 a1, u64 a2,
                     u64 a3, u64 a4, u64 a5)
{
    if (number >= SYS_MAX || !syscall_table[number]) {
        kprintf("[syscall] unknown number %lu\n", (unsigned long)number);
        return -1;
    }
    return syscall_table[number](a0, a1, a2, a3, a4, a5);
}
