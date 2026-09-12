/* ============================================================================
 * NetrdorvOS - 极简 printf（调试用）
 * 阶段1配套：仅支持 %c %s %d %u %x %p %%
 * ============================================================================ */

#include <driver/uart.h>
#include <types.h>

static void print_uint(u64 val, int base, int upper)
{
    char buf[32];
    static const char *digits_lower = "0123456789abcdef";
    static const char *digits_upper = "0123456789ABCDEF";
    const char *digits = upper ? digits_upper : digits_lower;
    int i = 0;

    if (val == 0) {
        uart_putc('0');
        return;
    }
    while (val && i < (int)sizeof(buf)) {
        buf[i++] = digits[val % base];
        val /= base;
    }
    while (i > 0)
        uart_putc(buf[--i]);
}

void kprintf(const char *fmt, ...)
{
    __builtin_va_list ap;
    __builtin_va_start(ap, fmt);

    while (*fmt) {
        if (*fmt != '%') {
            uart_putc(*fmt++);
            continue;
        }
        fmt++;
        switch (*fmt) {
        case 'c': {
            int c = __builtin_va_arg(ap, int);
            uart_putc((char)c);
            break;
        }
        case 's': {
            const char *s = __builtin_va_arg(ap, const char *);
            if (!s)
                s = "(null)";
            uart_puts(s);
            break;
        }
        case 'd': {
            int v = __builtin_va_arg(ap, int);
            if (v < 0) {
                uart_putc('-');
                print_uint((u64)(-(long long)v), 10, 0);
            } else {
                print_uint((u64)v, 10, 0);
            }
            break;
        }
        case 'u':
            print_uint((u64)__builtin_va_arg(ap, unsigned int), 10, 0);
            break;
        case 'x':
            print_uint((u64)__builtin_va_arg(ap, unsigned int), 16, 0);
            break;
        case 'X':
            print_uint((u64)__builtin_va_arg(ap, unsigned int), 16, 1);
            break;
        case 'p':
            uart_puts("0x");
            print_uint((u64)__builtin_va_arg(ap, void *), 16, 0);
            break;
        case 'l':
            fmt++;
            if (*fmt == 'x' || *fmt == 'X') {
                print_uint((u64)__builtin_va_arg(ap, unsigned long),
                           16, *fmt == 'X');
            } else if (*fmt == 'u') {
                print_uint((u64)__builtin_va_arg(ap, unsigned long), 10, 0);
            } else if (*fmt == 'd') {
                long v = __builtin_va_arg(ap, long);
                if (v < 0) {
                    uart_putc('-');
                    print_uint((u64)(-v), 10, 0);
                } else {
                    print_uint((u64)v, 10, 0);
                }
            } else {
                uart_putc('%');
                uart_putc('l');
                uart_putc(*fmt);
            }
            break;
        case '%':
            uart_putc('%');
            break;
        default:
            uart_putc('%');
            uart_putc(*fmt);
            break;
        }
        if (*fmt)
            fmt++;
    }

    __builtin_va_end(ap);
}
