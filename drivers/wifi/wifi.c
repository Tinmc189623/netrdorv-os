/* ============================================================================
 * NetrdorvOS - WiFi 驱动 (阶段5)
 * 占位实现：完成接口框架，驱动实现阶段5完成
 * ============================================================================ */

#include <driver/wifi.h>
#include <types.h>

int wifi_init(void)
{
    /* TODO(阶段5): SDIO 初始化、WiFi 固件加载 */
    return 0;
}

int wifi_scan(wifi_ap_t *aps, u32 max_aps)
{
    /* TODO(阶段5): 扫描并填充热点列表 */
    (void)aps; (void)max_aps;
    return 0;
}

int wifi_connect(const char *ssid, const char *password)
{
    /* TODO(阶段5): 关联 AP、DHCP 获取地址 */
    (void)ssid; (void)password;
    return 0;
}

void wifi_disconnect(void)
{
    /* TODO(阶段5) */
}

int wifi_is_connected(void)
{
    /* TODO(阶段5) */
    return 0;
}

u32 wifi_get_ip(void)
{
    /* TODO(阶段5) */
    return 0;
}
