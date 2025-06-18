/*
 Copyright 2025 Ivan Somov

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include "pch.hpp"
#include "WiFiManager.hpp"

#include "LoopManager.hpp"
#include "Message.hpp"

#include "esp_wifi.h"
#include "nvs.h"
#include "nvs_flash.h"

using namespace idf::event;

namespace kopter {
#define WIFI_CHECK_THROW(err) CHECK_THROW_WITH(err, WiFiException)

#define WIFI_CHANNEL CONFIG_AP_WIFI_CHANNEL
#if CONFIG_WIFI_MODE_SOFTAP
#define WIFI_MODE WIFI_MODE_AP
#else
#define WIFI_MODE WIFI_MODE_STA
#endif

constexpr std::string_view TAG = "[WiFiManager]";

WiFiException::WiFiException(esp_err_t error) : KopterException(error)
{
}

WiFiManager::WiFiManager()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        WIFI_CHECK_THROW(nvs_flash_erase());
        WIFI_CHECK_THROW(nvs_flash_init());
    }

    set_wifi_config();
    WIFI_CHECK_THROW(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    WIFI_CHECK_THROW(esp_wifi_start());
    WIFI_CHECK_THROW(esp_wifi_set_channel(WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE));
}

WiFiManager::~WiFiManager()
{
#if CONFIG_WIFI_MODE_STATION
    WIFI_CHECK_THROW(esp_wifi_disconnect());
#endif
    WIFI_CHECK_THROW(esp_wifi_stop());
}

WiFiManager &WiFiManager::get_instance(LoopManager *p_loop_manager)
{
    static WiFiManager instance;

    if (instance.m_loop_manager == nullptr && p_loop_manager != nullptr) {
        instance.m_loop_manager = p_loop_manager;
    }

    return instance;
}

void WiFiManager::set_wifi_config()
{
    wifi_init_config_t init_cfg = WIFI_INIT_CONFIG_DEFAULT();
    WIFI_CHECK_THROW(esp_wifi_init(&init_cfg));
    WIFI_CHECK_THROW(esp_wifi_set_mode(WIFI_MODE));
}

} // namespace kopter