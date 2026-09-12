/* ============================================================================
 * NetrdorvOS - SMP 实现
 * ============================================================================ */

#include <arch/smp.h>
#include <driver/uart.h>

per_cpu_t cpu_data[MAX_HARTS];
static u32 cpu_count = 1;

void smp_init(void)
{
    cpu_data[0].hart_id = 0;
    cpu_data[0].cpu_id = 0;
    cpu_data[0].boot_ready = 1;
    cpu_count = 1;
    kprintf("[smp] %u hart(s) online\n", cpu_count);
}

u32 smp_cpu_count(void) { return cpu_count; }

u32 smp_current_cpu(void)
{
    /* 简化：hart 0 = cpu 0；真实实现读 mhartid */
    return 0;
}

void smp_send_ipi(u32 target_cpu, u32 irq)
{
    (void)target_cpu; (void)irq;
    /* TODO: 写 msip 寄存器发软件中断 */
}
