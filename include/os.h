/* ============================================================================
 * NetrdorvOS - 通用宏与工具
 * ============================================================================ */

#ifndef __INCLUDE_OS_H__
#define __INCLUDE_OS_H__

#include <types.h>

/* 编译期断言 */
#define STATIC_ASSERT(cond) \
    typedef char static_assert_##__LINE__[(cond) ? 1 : -1]

/* 最小/最大/取整 */
#define MIN(a, b)       ((a) < (b) ? (a) : (b))
#define MAX(a, b)       ((a) > (b) ? (a) : (b))
#define CLAMP(x, lo, hi) ((x) < (lo) ? (lo) : ((x) > (hi) ? (hi) : (x)))
#define ALIGN_UP(x, a)  (((x) + (a) - 1) & ~((a) - 1))
#define ALIGN_DOWN(x, a) ((x) & ~((a) - 1))

/* 内存屏障（后续按需细化 acquire/release 语义） */
#define mb()            asm volatile("fence rw, rw" ::: "memory")
#define rmb()           asm volatile("fence r, r" ::: "memory")
#define wmb()           asm volatile("fence w, w" ::: "memory")

/* 暂停提示 */
#define cpu_pause()     asm volatile("nop" ::: "memory")
#define cpu_wfi()       asm volatile("wfi" ::: "memory")

#endif /* __INCLUDE_OS_H__ */
