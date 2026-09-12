/* ============================================================================
 * NetrdorvOS - 系统工具
 * bin/init.elf - 用户态初始化进程
 * bin/logcat   - 日志查看
 * bin/reboot   - 重启
 * ============================================================================ */

#include <user/unistd.h>
#include <types.h>

/* ---- /bin/init ---- */
void init_main(void)
{
    sys_write(1, "[init] NetrdorvOS user-space init starting\n", 0);
    /* TODO: 挂载文件系统、启动服务 */
    for (;;) sys_sleep(1000);
}

/* ---- /bin/logcat ---- */
void logcat_main(void)
{
    sys_write(1, "[logcat] streaming kernel logs...\n", 0);
    /* TODO: 从内核环形日志缓冲区读 */
    sys_exit(0);
}

/* ---- /bin/reboot ---- */
void reboot_main(void)
{
    sys_write(1, "[reboot] system reset\n", 0);
    /* TODO: 写 PMU 复位寄存器 */
    sys_exit(0);
}
