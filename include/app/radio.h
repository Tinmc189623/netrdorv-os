/* ============================================================================
 * NetrdorvOS - 收音机应用 (阶段6：触控UI、电台管理、音量控制)
 * ============================================================================ */

#ifndef __INCLUDE_APP_RADIO_H__
#define __INCLUDE_APP_RADIO_H__

#include <types.h>

#define RADIO_STATION_MAX   64
#define RADIO_NAME_LEN      64

typedef struct {
    char    name[RADIO_NAME_LEN];  /* 电台名称 */
    char    url[256];              /* 流媒体地址 */
    u8      favorite;              /* 是否收藏 */
} radio_station_t;

/* 应用状态机 */
typedef enum {
    RADIO_STATE_BOOT,       /* 开机初始化 */
    RADIO_STATE_STATION,    /* 频道列表页 */
    RADIO_STATE_PLAYING,    /* 播放页 */
    RADIO_STATE_SETTING,    /* 设置页 */
    RADIO_STATE_SLEEP,      /* 待机 */
} radio_state_t;

/* 初始化收音机应用 */
void radio_init(void);

/* 应用主循环（阶段6 由调度器创建为独立任务） */
void radio_main(void *arg);

/* 电台管理 */
int  radio_load_stations(radio_station_t *st, u32 max);   /* 本地缓存加载 */
int  radio_save_stations(const radio_station_t *st, u32 n);
void radio_play_station(u32 index);
void radio_stop(void);

#endif /* __INCLUDE_APP_RADIO_H__ */
