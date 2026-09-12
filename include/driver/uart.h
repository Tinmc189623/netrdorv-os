/* ============================================================================
 * NetrdorvOS - 串口驱动
 * 阶段1：串口驱动（调试打印）
 * ============================================================================ */

#ifndef __INCLUDE_DRIVER_UART_H__
#define __INCLUDE_DRIVER_UART_H__

#include <types.h>

/* 串口初始化（波特率、8N1） */
void uart_init(void);

/* 输出/输入单个字符 */
void uart_putc(char c);
int  uart_getc(void);           /* 无字符返回 -1 */

/* 输出字符串 */
void uart_puts(const char *s);

/* 调试输出（无完整 printf 时使用） */
void kprintf(const char *fmt, ...);

#endif /* __INCLUDE_DRIVER_UART_H__ */
