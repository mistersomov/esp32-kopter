#include "WiFiManager.hpp"
#include "LoopManager.hpp"

#include "esp_attr.h"
#include "esp_event_cxx.hpp"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "string.h"
#include <string_view>

using namespace idf::event;

#define WIFI_SSID CONFIG_WIFI_SSID
#define WIFI_PASS CONFIG_WIFI_PASSWORD
#define WIFI_CHANNEL CONFIG_WIFI_CHANNEL
#define WIFI_MAX_STA_CONN CONFIG_MAX_STA_CONN

namespace kopter {

constexpr std::string_view TAG = "[WiFiManager]";

WiFiManager::WiFiManager()
{
}

WiFiManager::~WiFiManager()
{
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

void WiFiManager::init_softap()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    ESP_ERROR_CHECK(esp_netif_init());

    esp_netif_create_default_wifi_ap();

    wifi_init_config_t init_cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&init_cfg));

    wifi_config_t wifi_config = {};
    strcpy(reinterpret_cast<char *>(wifi_config.ap.ssid), WIFI_SSID);
    strcpy(reinterpret_cast<char *>(wifi_config.ap.password), WIFI_PASS);
    wifi_config.ap.ssid_len = strlen(WIFI_SSID);
    wifi_config.ap.channel = WIFI_CHANNEL;
    wifi_config.ap.authmode = WIFI_AUTH_WPA2_PSK;
    wifi_config.ap.sae_pwe_h2e = WPA3_SAE_PWE_BOTH;
    wifi_config.ap.max_connection = WIFI_MAX_STA_CONN;
    wifi_config.ap.pmf_cfg.required = true;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_start());
}

} // namespace kopter