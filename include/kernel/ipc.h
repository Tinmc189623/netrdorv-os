/* ============================================================================
 * NetrdorvOS / Nexera Kernel - POSIX 信号 + IPC 同步原语
 * ============================================================================ */

#ifndef __INCLUDE_KERNEL_IPC_H__
#define __INCLUDE_KERNEL_IPC_H__

#include <types.h>

/* ---- POSIX 信号 ---- */
#define SIGHUP      1
#define SIGINT      2
#define SIGPIPE     13
#define SIGALRM     14
#define SIGTERM     15
#define SIGCHLD     17
#define SIGUSR1     10
#define SIGUSR2     12

#define SIG_MAX     32

typedef void (*sighandler_t)(int);

int  sys_kill(u64 pid, int sig);
int  sys_signal(int sig, sighandler_t handler);
void signal_deliver(u64 pid, int sig);

/* ---- 互斥锁（自旋） ---- */
typedef struct {
    volatile u32 locked;
} mutex_t;

#define MUTEX_INIT  { .locked = 0 }

void mutex_lock(mutex_t *m);
void mutex_unlock(mutex_t *m);

/* ---- 信号量 ---- */
typedef struct {
    volatile s32 value;
} sem_t;

void sem_init(sem_t *s, s32 value);
void sem_wait(sem_t *s);
void sem_post(sem_t *s);

#endif
