/* ============================================================================
 * NetrdorvOS - IPC 实现
 * 信号分发 + 自旋锁 + 信号量
 * ============================================================================ */

#include <kernel/ipc.h>
#include <kernel/sched.h>
#include <driver/uart.h>

/* 每个进程的信号处理器表 */
static sighandler_t signal_table[64][SIG_MAX];

int sys_kill(u64 pid, int sig)
{
    (void)pid;
    if (sig <= 0 || sig >= SIG_MAX) return -1;
    kprintf("[ipc] kill pid=%lu sig=%d\n", (unsigned long)pid, sig);
    return 0;
}

int sys_signal(int sig, sighandler_t handler)
{
    if (sig <= 0 || sig >= SIG_MAX) return -1;
    u64 pid = sched_current()->id;
    signal_table[pid][sig] = handler;
    return 0;
}

void signal_deliver(u64 pid, int sig)
{
    sighandler_t h = signal_table[pid][sig];
    if (h) h(sig);
}

void mutex_lock(mutex_t *m)
{
    while (__sync_lock_test_and_set(&m->locked, 1))
        ;
}

void mutex_unlock(mutex_t *m)
{
    __sync_lock_release(&m->locked, 0);
}

void sem_init(sem_t *s, s32 value) { s->value = value; }

void sem_wait(sem_t *s)
{
    while (__sync_lock_test_and_set(&s->value, s->value - 1) <= 0) {
        sched_yield();
    }
}

void sem_post(sem_t *s)
{
    __sync_add_and_fetch(&s->value, 1);
}
