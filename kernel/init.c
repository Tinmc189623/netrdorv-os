/* ============================================================================
 * NetrdorvOS - 内核 C 入口 (S-mode, Limine)
 *
 * 启动流程：
 *   Limine → mmu → trap → timer → pmm/heap → vfs → modules
 *           → exec /bin/init.elf（用户态）
 * ============================================================================ */

#include <types.h>
#include <os.h>
#include <arch/riscv.h>
#include <arch/mmu.h>
#include <arch/trap.h>
#include <arch/timer.h>
#include <kernel/mm.h>
#include <kernel/sched.h>
#include <kernel/vfs.h>
#include <kernel/module.h>
#include <kernel/syscall.h>
#include <driver/uart.h>
#include <lib/string.h>

extern char __kernel_end[];

void kmain(void)
{
    kprintf("\n========================================\n");
    kprintf("  NetrdorvOS / Nexera Kernel (NK) v0.1.0\n");
    kprintf("  S-mode, Limine, RV64GC\n");
    kprintf("========================================\n");

    /* ---- 1. MMU (Sv39 恒等映射) ---- */
    mmu_init();

    /* ---- 2. 中断 ---- */
    trap_init();

    /* ---- 3. 定时器 ---- */
    timer_init();

    /* ---- 4. 物理内存 + 堆 ---- */
    pmm_init(0, 0);
    kheap_init((void *)0x82400000UL, 32UL << 20);

    /* ---- 5. VFS 层 ---- */
    vfs_init();

    /* ---- 6. 调度器 ---- */
    sched_init();

    /* ---- 7. 加载驱动模块（社区 .ko / 内置占位） ---- */
    module_init_all();

    /* ---- 8. 启动用户态 init 进程 ---- */
    /* TODO: 从 FAT32 读取 /bin/init.elf，elf_load_user 后进入 U 态 */
    kprintf("[init] starting /bin/init.elf (TODO: filesystem load)\n");

    kprintf("[init] NetrdorvOS ready\n\n");

    /* 使能 S 态中断，进入调度 */
    csr_set(sstatus, SSTATUS_SIE);
    sched_run();
}
