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
#include "StringUtils.hpp"
#include "WiFiException.hpp"

#include "nvs.h"
#include "nvs_flash.h"

namespace kopter {

// AP Configuration
#define WIFI_AP_SSID CONFIG_WIFI_AP_SSID
#define WIFI_AP_PASSWD CONFIG_WIFI_AP_PASSWORD
#define MAX_STA_CONN CONFIG_MAX_STA_CONN_AP
#define WIFI_AP_CHANNEL CONFIG_WIFI_AP_CHANNEL

// STA Configuration
#define WIFI_STA_SSID CONFIG_WIFI_REMOTE_AP_SSID
#define WIFI_STA_PASSWD CONFIG_WIFI_REMOTE_AP_PASSWORD
#define MAXIMUM_RETRY CONFIG_MAXIMUM_STA_RETRY
#define LONG_RANGE CONFIG_ENABLE_LONG_RANGE

namespace {
constexpr uint16_t RECONNECT_DELAY = 1000;
constexpr std::string_view TAG = "[WiFiManager]";
} // namespace

WiFiManager::WiFiManager(LoopManager *p_loop_manager) : m_loop_manager{p_loop_manager}, m_retry_count{0}
{
}

WiFiManager::~WiFiManager()
{
    check_call<WiFiException>([&]() { esp_wifi_disconnect(); });
    check_call<WiFiException>([&]() { esp_wifi_stop(); });
    if (m_netif_ap) {
        esp_netif_destroy(m_netif_ap);
    }
    if (m_netif_sta) {
        esp_netif_destroy(m_netif_sta);
    }
}

WiFiManager &WiFiManager::get_instance(LoopManager *p_loop_manager)
{
    static std::once_flag flag;
    static std::unique_ptr<WiFiManager> instance;

    std::call_once(flag, [&]() {
        assert(p_loop_manager != nullptr && "LoopManager must not be null on first call");
        instance.reset(new WiFiManager(p_loop_manager));
    });

    return *instance;
}

void WiFiManager::init()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        check_call<WiFiException>([]() { nvs_flash_erase(); });
        check_call<WiFiException>([]() { nvs_flash_init(); });
    }

    check_call<WiFiException>([]() { esp_netif_init(); });

    set_event_handler();
    set_wifi_config();
    check_call<WiFiException>([]() { esp_wifi_set_storage(WIFI_STORAGE_RAM); });
    check_call<WiFiException>([]() { esp_wifi_start(); });
}

bool WiFiManager::is_ssid_empty() const
{
    return WIFI_STA_SSID == nullptr || std::strlen(WIFI_STA_SSID) == 0;
}

void WiFiManager::set_wifi_config()
{
    wifi_init_config_t init_cfg = WIFI_INIT_CONFIG_DEFAULT();
    check_call<WiFiException>([&]() { esp_wifi_init(&init_cfg); });
    check_call<WiFiException>([&]() { esp_wifi_set_mode(WIFI_MODE_APSTA); });
    set_wifi_ap_config();
    set_wifi_sta_config();
}

void WiFiManager::set_wifi_ap_config()
{
    m_netif_ap = esp_netif_create_default_wifi_ap();

    std::size_t pwd_len = std::strlen(WIFI_AP_PASSWD);

    if (pwd_len > 0 && pwd_len < 8) {
        ESP_LOGE(TAG.data(), "Password is too short: %zu characters", pwd_len);
        throw WiFiException(ESP_ERR_INVALID_SIZE);
    }

    wifi_config_t ap_cfg{};
    ap_cfg.ap.ssid_len = std::strlen(WIFI_AP_SSID);
    ap_cfg.ap.channel = WIFI_AP_CHANNEL;
    ap_cfg.ap.max_connection = MAX_STA_CONN;
    ap_cfg.ap.authmode = get_auth_mode();
    ap_cfg.ap.pmf_cfg.required = false;
    StringUtils::copy(ap_cfg.ap.ssid, WIFI_AP_SSID);
    StringUtils::copy(ap_cfg.ap.password, WIFI_AP_PASSWD);

    check_call<WiFiException>([&ap_cfg]() { esp_wifi_set_config(WIFI_IF_AP, &ap_cfg); });
}

void WiFiManager::set_wifi_sta_config()
{
    m_netif_sta = esp_netif_create_default_wifi_sta();

    wifi_config_t sta_cfg{};
    sta_cfg.sta.scan_method = WIFI_FAST_SCAN;
    sta_cfg.sta.threshold.authmode = get_auth_mode();
    StringUtils::copy(sta_cfg.sta.ssid, WIFI_STA_SSID);
    StringUtils::copy(sta_cfg.sta.password, WIFI_STA_PASSWD);

    check_call<WiFiException>([&sta_cfg]() { esp_wifi_set_config(WIFI_IF_STA, &sta_cfg); });
}

void WiFiManager::set_event_handler()
{
    m_event_regs.push_back(
        m_loop_manager->register_system_event(STA_START_EVENT,
                                              [this](const idf::event::ESPEvent &, void *) { sta_do_connect(); }));
    m_event_regs.push_back(
        m_loop_manager->register_system_event(STA_DISCONNECTED_EVENT,
                                              [this](const idf::event::ESPEvent &, void *) { sta_do_reconnect(); }));
    m_event_regs.push_back(
        m_loop_manager->register_system_event(IP_GOT_EVENT,
                                              [this](const idf::event::ESPEvent &, void *) { m_retry_count = 0; }));
}

void WiFiManager::sta_do_connect()
{
    check_call<WiFiException>([]() { esp_wifi_connect(); });
}

void WiFiManager::sta_do_reconnect()
{
    if (m_retry_count > MAXIMUM_RETRY) {
        ESP_LOGE(TAG.data(), "WiFi Connect failed %d times, stop reconnect.", m_retry_count);
        sta_do_disconnect();
        return;
    }

    ESP_LOGI(TAG.data(), "Trying to reconnect...");

    m_retry_count++;
    vTaskDelay(pdMS_TO_TICKS(RECONNECT_DELAY));
    sta_do_connect();
}

void WiFiManager::sta_do_disconnect()
{
    m_retry_count = 0;
    m_event_regs.clear();
}

constexpr wifi_auth_mode_t WiFiManager::get_auth_mode() const
{
#if CONFIG_WIFI_AUTH_WEP
    return WIFI_AUTH_WEP;
#elif CONFIG_WIFI_AUTH_WPA_PSK
    return WIFI_AUTH_WPA_PSK;
#elif CONFIG_WIFI_AUTH_WPA2_PSK
    return WIFI_AUTH_WPA2_PSK;
#elif CONFIG_WIFI_AUTH_WPA_WPA2_PSK
    return WIFI_AUTH_WPA_WPA2_PSK;
#elif CONFIG_WIFI_AUTH_WPA3_PSK
    return WIFI_AUTH_WPA3_PSK;
#elif CONFIG_WIFI_AUTH_WPA2_WPA3_PSK
    return WIFI_AUTH_WPA2_WPA3_PSK;
#elif CONFIG_WIFI_AUTH_WAPI_PSK
    return WIFI_AUTH_WAPI_PSK;
#else
    return WIFI_AUTH_OPEN;
#endif
}

} // namespace kopter