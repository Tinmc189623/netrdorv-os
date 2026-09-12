/* ============================================================================
 * NetrdorvOS - UART 串口驱动 (/dev/ttyS0)
 * 16550 兼容，基址待按 SG2002 校准
 * ============================================================================ */

#include <driver/uart.h>
#include <kernel/vfs.h>
#include <kernel/module.h>
#include <arch/riscv.h>

#define UART_BASE       0x04140000UL

#define UART_THR        0x00
#define UART_RBR        0x00
#define UART_IER        0x01
#define UART_FCR        0x02
#define UART_LCR        0x03
#define UART_MCR        0x04
#define UART_LSR        0x05
#define UART_DLL        0x00
#define UART_DLM        0x01

#define LSR_THRE        BIT(5)
#define LSR_DR          BIT(0)

static inline void uart_reg_write(u32 off, u8 v) { *(volatile u8 *)(UART_BASE+off) = v; }
static inline u8  uart_reg_read(u32 off)         { return *(volatile u8 *)(UART_BASE+off); }

void uart_init(void)
{
    uart_reg_write(UART_IER, 0x00);
    uart_reg_write(UART_LCR, 0x80);
    uart_reg_write(UART_DLL, 0x01);
    uart_reg_write(UART_DLM, 0x00);
    uart_reg_write(UART_LCR, 0x03);
    uart_reg_write(UART_FCR, 0x01);
}

void uart_putc(char c)
{
    while (!(uart_reg_read(UART_LSR) & LSR_THRE)) ;
    uart_reg_write(UART_THR, (u8)c);
}

int uart_getc(void)
{
    if (!(uart_reg_read(UART_LSR) & LSR_DR)) return -1;
    return (int)uart_reg_read(UART_RBR);
}

void uart_puts(const char *s)
{
    while (*s) {
        if (*s == '\n') uart_putc('\r');
        uart_putc(*s++);
    }
}

/* ---- VFS: /dev/ttyS0 ---- */
static int tty_open(const char *p, int f) { (void)p; (void)f; return 0; }
static int tty_read(int fd, void *buf, size_t n) {
    (void)fd;
    for (size_t i = 0; i < n; i++) {
        int c = uart_getc();
        if (c < 0) return (int)i;
        ((u8 *)buf)[i] = (u8)c;
    }
    return (int)n;
}
static int tty_write(int fd, const void *buf, size_t n) {
    (void)fd;
    for (size_t i = 0; i < n; i++) uart_putc(((const char *)buf)[i]);
    return (int)n;
}
static int tty_ioctl(int fd, unsigned long r, void *a) { (void)fd;(void)r;(void)a; return 0; }
static int tty_close(int fd) { (void)fd; return 0; }

static file_ops_t tty_ops = {
    .open = tty_open, .read = tty_read, .write = tty_write,
    .ioctl = tty_ioctl, .close = tty_close,
};

static int tty_module_init(void) { uart_init(); return 0; }
static void tty_module_exit(void) {}

MODULE_DEFINE(ttyS0, tty_module_init, tty_module_exit, &tty_ops);
