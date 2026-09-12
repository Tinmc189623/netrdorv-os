/* ============================================================================
 * NetrdorvOS - 系统调用号定义
 * 用户态通过 ecall 进入，参数 a0..a5，调用号 a7
 * ============================================================================ */

#ifndef __INCLUDE_KERNEL_SYSCALL_H__
#define __INCLUDE_KERNEL_SYSCALL_H__

#include <types.h>

#define SYS_OPEN        0
#define SYS_CLOSE       1
#define SYS_READ        2
#define SYS_WRITE       3
#define SYS_IOCTL       4
#define SYS_EXIT        5
#define SYS_SCHED_YIELD 6
#define SYS_SLEEP       7
#define SYS_GETPID      8
#define SYS_FORK        9
#define SYS_EXECVE      10
#define SYS_BRK         11
#define SYS_MMAP        12
#define SYS_OPENAT      13

#define SYS_MAX         16

/* syscall 处理入口（由 trap.c 调用） */
u64 syscall_dispatch(u64 number, u64 a0, u64 a1, u64 a2,
                     u64 a3, u64 a4, u64 a5);

#endif /* __INCLUDE_KERNEL_SYSCALL_H__ */
