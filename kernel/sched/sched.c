/* ============================================================================
 * NetrdorvOS - 多任务调度器实现 (阶段2)
 * 简单轮转调度：就绪队列 + 上下文切换
 * ============================================================================ */

#include <kernel/sched.h>
#include <kernel/mm.h>
#include <types.h>
#include <os.h>
#include <driver/uart.h>
#include <lib/string.h>

/* 上下文切换汇编接口 */
extern void sched_context_switch(u64 *old_context_sp, u64 new_context_sp);
extern void task_first_entry(void);

/* 最大任务数 */
#define MAX_TASKS 16

static task_t task_table[MAX_TASKS];
static task_t *ready_queue = NULL;
static task_t *current_task = NULL;
static u64 next_task_id = 1;
static volatile u64 system_tick = 0;

/* ---------- 就绪队列操作 ---------- */
static void ready_enqueue(task_t *t)
{
    t->state = TASK_READY;
    t->next = NULL;
    if (!ready_queue) {
        ready_queue = t;
        return;
    }
    task_t *p = ready_queue;
    while (p->next)
        p = p->next;
    p->next = t;
}

static task_t *ready_dequeue(void)
{
    task_t *t = ready_queue;
    if (t)
        ready_queue = t->next;
    return t;
}

/* ---------- Idle 任务 ---------- */
static void idle_entry(void *arg)
{
    (void)arg;
    for (;;) {
        sched_yield();
        cpu_wfi();
    }
}

/* ---------- 初始化 ---------- */
void sched_init(void)
{
    /* 清空任务表 */
    for (int i = 0; i < MAX_TASKS; i++) {
        task_table[i].id = 0;
        task_table[i].state = TASK_EXITED;
        task_table[i].next = NULL;
    }
    ready_queue = NULL;
    current_task = NULL;
    system_tick = 0;

    /* 创建 idle 任务（id=0） */
    sched_create_task("idle", idle_entry, NULL, 0);
    kprintf("[sched] initialized, max_tasks=%d\n", MAX_TASKS);
}

/* ---------- 创建任务 ---------- */
int sched_create_task(const char *name, void (*entry)(void *), void *arg,
                      u64 priority)
{
    /* 找空闲槽位 */
    task_t *t = NULL;
    for (int i = 0; i < MAX_TASKS; i++) {
        if (task_table[i].state == TASK_EXITED) {
            t = &task_table[i];
            break;
        }
    }
    if (!t)
        return -1;

    /* 分配栈 */
    void *stack_mem = kmalloc(TASK_STACK_SIZE);
    if (!stack_mem)
        return -2;

    t->id = next_task_id++;
    t->priority = priority;
    t->state = TASK_READY;
    t->stack = (u64 *)stack_mem;
    t->stack_size = TASK_STACK_SIZE;
    t->stack_top = (u64)stack_mem + TASK_STACK_SIZE;
    t->wakeup_tick = 0;
    t->next = NULL;

    strncpy(t->name, name, TASK_NAME_LEN - 1);
    t->name[TASK_NAME_LEN - 1] = '\0';

    /*
     * 在新任务栈上构造初始上下文帧。
     * 栈顶（高地址）向下布局：
     *   [stack_top - 144]  entry   (task_first_entry 取)
     *   [stack_top - 136]  arg     (task_first_entry 取)
     *   [stack_top - 128]  ra = task_first_entry
     *   [stack_top - 120]  s0 = 0
     *   ...                s1..s11 = 0
     *   [stack_top - 16]   tp = 0
     */
    u64 *sp = (u64 *)t->stack_top;
    sp[-18] = (u64)entry;              /* stack_top - 144 */
    sp[-17] = (u64)arg;                /* stack_top - 136 */
    sp[-16] = (u64)task_first_entry;   /* ra */
    for (int i = 1; i <= 14; i++)      /* s0..s11, gp, tp = 0 */
        sp[-i] = 0;

    /* context_sp 指向 context frame 底部 */
    t->context_sp = (u64)(sp - 16);

    ready_enqueue(t);
    kprintf("[sched] task '%s' (id=%lu) created, stack=%p top=0x%lx\n",
            name, (unsigned long)t->id, stack_mem,
            (unsigned long)t->stack_top);
    return (int)t->id;
}

/* ---------- 让出 CPU ---------- */
void sched_yield(void)
{
    if (!current_task)
        return;

    /* 找下一个就绪任务 */
    task_t *next = ready_dequeue();
    if (!next)
        return;

    /* 把当前任务放回就绪队列（若仍就绪） */
    if (current_task->state == TASK_RUNNING) {
        current_task->state = TASK_READY;
        ready_enqueue(current_task);
    }

    task_t *prev = current_task;
    current_task = next;
    next->state = TASK_RUNNING;

    if (prev != next)
        sched_context_switch(&prev->context_sp, next->context_sp);
}

/* ---------- 睡眠 ---------- */
void sched_sleep(u64 ticks)
{
    if (!current_task || ticks == 0)
        return;
    current_task->state = TASK_SLEEPING;
    current_task->wakeup_tick = system_tick + ticks;
    sched_yield();
}

/* ---------- 时钟滴答 ---------- */
void sched_tick(void)
{
    system_tick++;

    /* 检查睡眠任务是否到期 */
    for (int i = 0; i < MAX_TASKS; i++) {
        task_t *t = &task_table[i];
        if (t->state == TASK_SLEEPING && t->wakeup_tick <= system_tick) {
            t->state = TASK_READY;
            ready_enqueue(t);
        }
    }
}

/* ---------- 调度主循环 ---------- */
void sched_run(void)
{
    kprintf("[sched] scheduler starting\n");
    for (;;)
        sched_yield();
}

task_t *sched_current(void)
{
    return current_task;
}

void sched_halt(void)
{
    if (current_task) {
        current_task->state = TASK_EXITED;
        kfree((void *)current_task->stack);
    }
    sched_yield();
}
