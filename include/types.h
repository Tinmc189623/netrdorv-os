/* ============================================================================
 * NetrdorvOS - 基础类型定义
 * ============================================================================ */

#ifndef __INCLUDE_TYPES_H__
#define __INCLUDE_TYPES_H__

typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long       u64;
typedef signed char         s8;
typedef signed short        s16;
typedef signed int          s32;
typedef signed long         s64;
typedef unsigned long       uintptr_t;
typedef long                intptr_t;
typedef unsigned long       size_t;
typedef long                ssize_t;
typedef unsigned long       off_t;

/* C23 起 bool 是关键字，老标准用 _Bool */
#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 202000L
typedef _Bool               bool;
#endif

#define true    1
#define false   0
#define NULL    ((void *)0)

#define BIT(n)          (1UL << (n))

/* 取结构体成员偏移 / 由成员指针反推结构体 */
#define offsetof(type, member)  ((size_t)&(((type *)0)->member))
#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#endif /* __INCLUDE_TYPES_H__ */
