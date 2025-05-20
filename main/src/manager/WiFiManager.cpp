#include "manager/WiFiManager.hpp"
#include "event/WiFiEvents.hpp"
#include "manager/LoopManager.hpp"
#include "Message.hpp"

#include "nvs.h"
#include "nvs_flash.h"
#include "string.h"
#include <cstring>
#include <string_view>

using namespace idf::event;

#define WIFI_CHANNEL CONFIG_AP_WIFI_CHANNEL
#if CONFIG_WIFI_MODE_SOFTAP
#define WIFI_MODE WIFI_MODE_AP
#else
#define WIFI_MODE WIFI_MODE_STA
#endif

namespace kopter {

constexpr std::string_view TAG = "[WiFiManager]";

WiFiManager::WiFiManager()
{
}

WiFiManager::~WiFiManager()
{
#if CONFIG_WIFI_MODE_STATION
    ESP_ERROR_CHECK(esp_wifi_disconnect());
#endif
    ESP_ERROR_CHECK(esp_wifi_stop());
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

    set_wifi_config();
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_set_channel(WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE));
}

void WiFiManager::set_wifi_config()
{
    wifi_init_config_t init_cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&init_cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE));
}

} // namespace kopter