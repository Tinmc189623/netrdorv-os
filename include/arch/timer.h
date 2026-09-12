/* ============================================================================
 * NetrdorvOS - 系统定时器 (CLINT mtime/mtimecmp)
 * 阶段1：时钟、定时器、系统滴答
 * ============================================================================ */

#ifndef __INCLUDE_ARCH_TIMER_H__
#define __INCLUDE_ARCH_TIMER_H__

#include <types.h>

/* 初始化定时器并启动系统滴答 */
void timer_init(void);

/* 获取当前 mtime 值 */
u64 timer_get_ticks(void);

/* 获取当前频率（Hz） */
u64 timer_get_freq(void);

/* 设置下一次时钟中断（由中断处理调用） */
void timer_set_next(void);

/* 忙延时（毫秒） */
void timer_delay_ms(u64 ms);

#endif /* __INCLUDE_ARCH_TIMER_H__ */
