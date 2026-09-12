/* ============================================================================
 * NetrdorvOS - 多任务调度器 (阶段2)
 * 简单的轮转调度器：就绪队列 + 上下文切换
 * ============================================================================ */

#ifndef __INCLUDE_KERNEL_SCHED_H__
#define __INCLUDE_KERNEL_SCHED_H__

#include <types.h>

#define TASK_NAME_LEN   32
#define TASK_STACK_SIZE 8192

typedef enum {
    TASK_READY = 0,
    TASK_RUNNING,
    TASK_BLOCKED,
    TASK_SLEEPING,
    TASK_EXITED,
} task_state_t;

typedef struct task {
    u64             id;
    char            name[TASK_NAME_LEN];
    task_state_t    state;
    u64             priority;
    u64            *stack;          /* 任务栈底（低地址） */
    u64             stack_size;
    u64             stack_top;       /* 栈顶（高地址），即切换时 sp */
    u64             context_sp;      /* 保存的 sp（指向栈上 context frame） */
    u64             wakeup_tick;    /* 睡眠唤醒时钟 */
    struct task    *next;            /* 就绪队列链表 */
} task_t;

/* 初始化调度器（创建 idle 任务） */
void sched_init(void);

/* 创建任务（返回任务 id，负数表示失败） */
int sched_create_task(const char *name, void (*entry)(void *), void *arg,
                      u64 priority);

/* 让出 CPU（调度下一个就绪任务） */
void sched_yield(void);

/* 当前任务睡眠 n 个时钟滴答 */
void sched_sleep(u64 ticks);

/* 调度器主循环（在 idle/boot 任务中调用，永不返回） */
void sched_run(void);

/* 获取当前任务 */
task_t *sched_current(void);

/* 时钟滴答（由 timer 中断调用） */
void sched_tick(void);

/* 终止当前任务（sys_exit 调用） */
void sched_halt(void);

#endif /* __INCLUDE_KERNEL_SCHED_H__ */
