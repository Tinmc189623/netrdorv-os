/* ============================================================================
 * NetrdorvOS - I2C 触控驱动 (阶段3：触点坐标解析、手势识别)
 * ============================================================================ */

#ifndef __INCLUDE_DRIVER_TOUCH_H__
#define __INCLUDE_DRIVER_TOUCH_H__

#include <types.h>

typedef enum {
    GESTURE_NONE = 0,
    GESTURE_TAP,            /* 点击选台 */
    GESTURE_SWIPE_UP,       /* 上滑 */
    GESTURE_SWIPE_DOWN,     /* 下滑 */
    GESTURE_SWIPE_LEFT,     /* 左滑 */
    GESTURE_SWIPE_RIGHT,    /* 右滑 */
} gesture_t;

typedef struct {
    u16 x;
    u16 y;
    u8  pressed;            /* 是否按下 */
} touch_point_t;

/* 初始化 I2C 触控 */
int touch_init(void);

/* 读取当前触点（阻塞至有数据或超时） */
int touch_read(touch_point_t *pt);

/* 获取最近一次手势（非阻塞） */
gesture_t touch_get_gesture(void);

#endif /* __INCLUDE_DRIVER_TOUCH_H__ */
