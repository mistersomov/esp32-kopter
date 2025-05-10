#include "WiFiManager.hpp"
#include "LoopManager.hpp"

#include "esp_event_cxx.hpp"
#include "esp_now.h"
#include "esp_wifi.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "string.h"
#include <string_view>

using namespace idf::event;

#define WIFI_SSID CONFIG_AP_WIFI_SSID
#define WIFI_PASS CONFIG_AP_WIFI_PASSWORD
#if CONFIG_WIFI_MODE_STATION_SOFTAP
#define WIFI_CHANNEL CONFIG_AP_WIFI_CHANNEL
#define WIFI_MAX_STA_CONN CONFIG_AP_MAX_STA_CONN
#endif

namespace kopter {

constexpr std::string_view TAG = "[WiFiManager]";

static void esp_now_rv_cb(const esp_now_recv_info_t *esp_now_info, const uint8_t *data, int data_len)
{
}

WiFiManager::WiFiManager()
{
}

WiFiManager::~WiFiManager()
{
    ESP_ERROR_CHECK(esp_now_deinit());
    ESP_ERROR_CHECK(esp_wifi_stop());
    ESP_ERROR_CHECK(esp_netif_deinit());
}

WiFiManager &WiFiManager::get_instance(LoopManager *p_loop_manager)
{
    static WiFiManager instance;

    if (instance.m_loop_manager == nullptr && p_loop_manager != nullptr) {
        instance.m_loop_manager = p_loop_manager;
    }

    return instance;
}

void WiFiManager::init()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    ESP_ERROR_CHECK(esp_netif_init());

#if CONFIG_WIFI_MODE_STATION
    esp_netif_create_default_wifi_sta();
#else
    esp_netif_create_default_wifi_ap();
#endif

    set_wifi_config();

    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_start());

#if CONFIG_ENABLE_LONG_RANGE
    ESP_ERROR_CHECK(esp_wifi_set_protocol(
        ESPNOW_WIFI_IF, WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N | WIFI_PROTOCOL_LR
    ));
#endif

    init_esp_now();

#if CONFIG_WIFI_MODE_STATION
    ESP_LOGI(TAG.data(), "wifi_init_sta finished.");
#else
    ESP_LOGI(
        TAG.data(), "wifi_init_softap finished. SSID:%s password:%s channel:%d", WIFI_SSID, WIFI_PASS, WIFI_CHANNEL
    );
#endif
}

void WiFiManager::init_esp_now()
{
    ESP_ERROR_CHECK(esp_now_init());
    // ESP_ERROR_CHECK(esp_now_register_send_cb());
    ESP_ERROR_CHECK(esp_now_register_recv_cb(esp_now_rv_cb));
}

void WiFiManager::set_wifi_config()
{
    wifi_init_config_t init_cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&init_cfg));

    wifi_config_t cfg = {};
#if CONFIG_WIFI_MODE_STATION
    strcpy(reinterpret_cast<char *>(cfg.sta.ssid), WIFI_SSID);
    strcpy(reinterpret_cast<char *>(cfg.sta.password), WIFI_PASS);

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &cfg));
#else
    strcpy(reinterpret_cast<char *>(cfg.ap.ssid), WIFI_SSID);
    strcpy(reinterpret_cast<char *>(cfg.ap.password), WIFI_PASS);
    cfg.ap.ssid_len = strlen(WIFI_SSID);
    cfg.ap.channel = WIFI_CHANNEL;
    cfg.ap.authmode = WIFI_AUTH_WPA2_PSK;
    cfg.ap.sae_pwe_h2e = WPA3_SAE_PWE_BOTH;
    cfg.ap.max_connection = WIFI_MAX_STA_CONN;
    cfg.ap.pmf_cfg.required = true;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &cfg));
#endif
}

} // namespace kopter