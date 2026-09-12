/* ============================================================================
 * NetrdorvOS - SBI 调用封装 (RV64)
 * 阶段1：SBI 调用、特权级切换
 *
 * 若后续采用 M 态 SBI 固件 + S 态内核的经典方案，
 * 本模块提供 ecall 封装。直接 M 态裸跑时本模块可不参与链接。
 * ============================================================================ */

#ifndef __INCLUDE_ARCH_SBI_H__
#define __INCLUDE_ARCH_SBI_H__

#include <types.h>

/* SBI v0.2 扩展 ID */
#define SBI_EXT_BASE        0x10
#define SBI_EXT_TIME        0x54494D45
#define SBI_EXT_IPI         0x735049
#define SBI_EXT_RFENCE      0x52464E43
#define SBI_EXT_HSM         0x48534D

/* SBI base 扩展函数 ID */
#define SBI_BASE_GET_SPEC_VERSION   0
#define SBI_BASE_GET_IMPL_ID        1
#define SBI_BASE_GET_IMPL_VERSION   2
#define SBI_BASE_PROBE_EXT          3
#define SBI_BASE_GET_MVENDORID      4
#define SBI_BASE_GET_MARCHID        5
#define SBI_BASE_GET_MIMPID         6

/* 遗留扩展（legacy）函数 ID */
#define SBI_CONSOLE_PUTCHAR  1
#define SBI_CONSOLE_GETCHAR  2
#define SBI_SHUTDOWN         8

/* SBI 返回状态 */
typedef struct {
    long error;     /* 错误码，0 表示成功 */
    long value;     /* 返回值 */
} sbi_ret_t;

/* 发起一次 SBI 调用（符合 RISC-V SBI v0.2 规范：eid 在 a7，fid 在 a6） */
static inline sbi_ret_t sbi_call(long eid, long fid, long arg0, long arg1,
                                 long arg2, long arg3, long arg4, long arg5)
{
    register long a0 asm("a0") = arg0;
    register long a1 asm("a1") = arg1;
    register long a2 asm("a2") = arg2;
    register long a3 asm("a3") = arg3;
    register long a4 asm("a4") = arg4;
    register long a5 asm("a5") = arg5;
    register long a6 asm("a6") = fid;
    register long a7 asm("a7") = eid;

    asm volatile("ecall"
                 : "+r"(a0), "+r"(a1)
                 : "r"(a2), "r"(a3), "r"(a4), "r"(a5), "r"(a6), "r"(a7)
                 : "memory");

    sbi_ret_t ret;
    ret.error = a0;
    ret.value = a1;
    return ret;
}

/* ---- 常用封装 ---- */

/* 查询 SBI 规范版本 */
static inline sbi_ret_t sbi_get_spec_version(void)
{
    return sbi_call(SBI_EXT_BASE, SBI_BASE_GET_SPEC_VERSION, 0,0,0,0,0,0);
}

/* 探测某扩展是否可用 */
static inline sbi_ret_t sbi_probe_extension(long ext_id)
{
    return sbi_call(SBI_EXT_BASE, SBI_BASE_PROBE_EXT, ext_id,0,0,0,0,0);
}

/* 串口输出一个字符（legacy） */
static inline long sbi_console_putchar(char ch)
{
    return sbi_call(SBI_CONSOLE_PUTCHAR, 0, ch,0,0,0,0,0).error;
}

/* 串口读取一个字符（legacy，无字符返回 -1） */
static inline long sbi_console_getchar(void)
{
    return sbi_call(SBI_CONSOLE_GETCHAR, 0, 0,0,0,0,0,0).error;
}

/* 关闭系统（legacy） */
static inline void sbi_shutdown(void)
{
    sbi_call(SBI_SHUTDOWN, 0, 0,0,0,0,0,0);
}

#endif /* __INCLUDE_ARCH_SBI_H__ */
