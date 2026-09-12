/* ============================================================================
 * NetrdorvOS - WiFi 驱动 (阶段5：SDIO WiFi 初始化、扫描、连接)
 * ============================================================================ */

#ifndef __INCLUDE_DRIVER_WIFI_H__
#define __INCLUDE_DRIVER_WIFI_H__

#include <types.h>

#define WIFI_SSID_MAX_LEN   32
#define WIFI_PASS_MAX_LEN   64
#define WIFI_AP_MAX         16

typedef struct {
    char    ssid[WIFI_SSID_MAX_LEN + 1];
    u8      bssid[6];
    s8      rssi;               /* 信号强度 dBm */
    u8      channel;
    u8      security;           /* 0=开放 1=WPA/WPA2 */
} wifi_ap_t;

/* 初始化 SDIO WiFi */
int wifi_init(void);

/* 扫描热点（阻塞，返回找到的数量） */
int wifi_scan(wifi_ap_t *aps, u32 max_aps);

/* 连接指定热点 */
int wifi_connect(const char *ssid, const char *password);

/* 断开连接 */
void wifi_disconnect(void);

/* 是否已连接 */
int wifi_is_connected(void);

/* 获取本机 IP（成功后有效） */
u32 wifi_get_ip(void);

#endif /* __INCLUDE_DRIVER_WIFI_H__ */
