/* ============================================================================
 * NetrdorvOS / Nexera Kernel - SMP 多核支持
 * Secondary HART 启动 + 每CPU数据 + IPI
 * ============================================================================ */

#ifndef __ARCH_RISCV64_SMP_H__
#define __ARCH_RISCV64_SMP_H__

#include <types.h>

#define MAX_HARTS  4

typedef struct {
    u32  hart_id;
    u32  cpu_id;
    u64  stack_top;
    u64  boot_ready;
    void *private_data;
} per_cpu_t;

extern per_cpu_t cpu_data[MAX_HARTS];

void smp_init(void);
u32  smp_cpu_count(void);
u32  smp_current_cpu(void);
void smp_send_ipi(u32 target_cpu, u32 irq);

#endif
