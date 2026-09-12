/* ============================================================================
 * NetrdorvOS - 系统定时器 (S-mode)
 * 通过 SBI 调用设置 stimecmp；CLINT mtime 恒等映射可读
 * ============================================================================ */

#include <arch/timer.h>
#include <arch/riscv.h>
#include <driver/uart.h>

#define CLINT_BASE      0x02000000UL
#define CLINT_MTIME     (CLINT_BASE + 0xBFF8)

#define RTC_FREQ_HZ     10000000UL
#define TICK_HZ         100

static volatile u64 *mtime_reg = (volatile u64 *)CLINT_MTIME;

/* SBI ecall 设置 timer */
static void sbi_set_timer(u64 stime_value)
{
    register u64 a0 asm("a0") = stime_value;
    register u64 a6 asm("a6") = 0;
    register u64 a7 asm("a7") = 0;
    asm volatile("ecall" :
                 "+r"(a0) : "r"(a6), "r"(a7) : "memory");
}

u64 timer_get_ticks(void) { return *mtime_reg; }
u64 timer_get_freq(void)  { return RTC_FREQ_HZ; }

void timer_set_next(void)
{
    sbi_set_timer(*mtime_reg + (RTC_FREQ_HZ / TICK_HZ));
}

void timer_init(void)
{
    sbi_set_timer(*mtime_reg + (RTC_FREQ_HZ / TICK_HZ));
    kprintf("[timer] S-mode timer, %lu Hz tick\n", (unsigned long)TICK_HZ);
}

void timer_delay_ms(u64 ms)
{
    u64 start = *mtime_reg;
    u64 wait = (ms * RTC_FREQ_HZ) / 1000;
    while ((*mtime_reg - start) < wait)
        ;
}
