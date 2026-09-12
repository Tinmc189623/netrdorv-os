/* ============================================================================
 * NetrdorvOS - 用户态 C 库极简封装
 * 通过 ecall 发起系统调用
 * ============================================================================ */

#ifndef __INCLUDE_USER_UNISTD_H__
#define __INCLUDE_USER_UNISTD_H__

#include <types.h>

static inline u64 syscall6(u64 num, u64 a0, u64 a1, u64 a2,
                           u64 a3, u64 a4, u64 a5)
{
    register u64 x10 asm("a0") = a0;
    register u64 x11 asm("a1") = a1;
    register u64 x12 asm("a2") = a2;
    register u64 x13 asm("a3") = a3;
    register u64 x14 asm("a4") = a4;
    register u64 x15 asm("a5") = a5;
    register u64 x17 asm("a7") = num;
    asm volatile("ecall" : "+r"(x10) :
                 "r"(x11), "r"(x12), "r"(x13), "r"(x14),
                 "r"(x15), "r"(x17) : "memory");
    return x10;
}

#define sys_open(path, flags)   syscall6(0, (u64)(path), (u64)(flags),0,0,0,0)
#define sys_close(fd)           syscall6(1, (u64)(fd), 0,0,0,0,0)
#define sys_read(fd,buf,n)      syscall6(2, (u64)(fd),(u64)(buf),(u64)(n),0,0,0)
#define sys_write(fd,buf,n)     syscall6(3, (u64)(fd),(u64)(buf),(u64)(n),0,0,0)
#define sys_ioctl(fd,req,arg)   syscall6(4, (u64)(fd),(u64)(req),(u64)(arg),0,0,0)
#define sys_exit(code)          syscall6(5, (u64)(code), 0,0,0,0,0)
#define sys_yield()             syscall6(6, 0,0,0,0,0,0)
#define sys_sleep(ticks)        syscall6(7, (u64)(ticks), 0,0,0,0,0)

#endif
