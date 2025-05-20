#include "WiFiManager.hpp"
#include "event/WiFiEvents.hpp"
#include "LoopManager.hpp"

#include "esp_now.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "string.h"
#include <cstring>
#include <memory>
#include <string_view>

using namespace idf::event;

#define WIFI_SSID CONFIG_AP_WIFI_SSID
#define WIFI_PASS CONFIG_AP_WIFI_PASSWORD
#define WIFI_CHANNEL CONFIG_AP_WIFI_CHANNEL
#if CONFIG_WIFI_MODE_SOFTAP
#define WIFI_MAX_STA_CONN CONFIG_AP_MAX_STA_CONN
#define WIFI_MODE WIFI_MODE_AP
#define ESPNOW_WIFI_IF WIFI_IF_AP
#define ESPNOW_PEER_MAC WIFI_IF_STA
#else
#define WIFI_MODE WIFI_MODE_STA
#define ESPNOW_WIFI_IF WIFI_IF_STA
#define ESPNOW_PEER_MAC WIFI_IF_AP
#endif
#define PMK CONFIG_PMK
#define LMK CONFIG_LMK

namespace kopter {
std::unique_ptr<ESPEventReg> wifi_started_event_reg;

constexpr std::string_view TAG = "[WiFiManager]";

static void esp_now_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status)
{
}

static void esp_now_rv_cb(const esp_now_recv_info_t *esp_now_info, const uint8_t *data, int data_len)
{
}

WiFiManager::WiFiManager()
{
}

WiFiManager::~WiFiManager()
{
    ESP_ERROR_CHECK(esp_now_deinit());

#if CONFIG_WIFI_MODE_STATION
    ESP_ERROR_CHECK(esp_wifi_disconnect());
#endif

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

    wifi_started_event_reg = m_loop_manager->register_system_event(event::WIFI_STARTED, [](const ESPEvent &, void *) {
        ESP_ERROR_CHECK(esp_wifi_connect());
    });

#if WIFI_MODE_STATION_SOFTAP
    esp_netif_create_default_wifi_ap();
#else
    esp_netif_create_default_wifi_sta();
#endif

    set_wifi_config();
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_start());
    init_esp_now();
}

void WiFiManager::init_esp_now()
{
    ESP_ERROR_CHECK(esp_now_init());
    ESP_ERROR_CHECK(esp_now_register_send_cb(esp_now_send_cb));
    ESP_ERROR_CHECK(esp_now_register_recv_cb(esp_now_rv_cb));
    ESP_ERROR_CHECK(add_broadcast_peer());
    ESP_ERROR_CHECK(esp_now_set_pmk(reinterpret_cast<const uint8_t *>(PMK)));
}

esp_err_t WiFiManager::add_broadcast_peer()
{
    esp_now_peer_info peer_info{};
    peer_info.channel = WIFI_CHANNEL;
    peer_info.ifidx = ESPNOW_WIFI_IF;
    peer_info.encrypt = false;

    uint8_t mac[6];
    esp_err_t ret = esp_wifi_get_mac(ESPNOW_PEER_MAC, mac);
    if (ret == ESP_OK) {
        std::memcpy(peer_info.peer_addr, mac, sizeof(mac));
        return esp_now_add_peer(&peer_info);
    }
    else {
        ESP_LOGE("MAC", "Failed to get MAC address: %s", esp_err_to_name(ret));
        return ESP_FAIL;
    }
}

void WiFiManager::set_wifi_config()
{
    wifi_init_config_t init_cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&init_cfg));

    wifi_config_t cfg = {};

#if WIFI_MODE_STATION_SOFTAP
    strcpy(reinterpret_cast<char *>(cfg.ap.ssid), WIFI_SSID);
    strcpy(reinterpret_cast<char *>(cfg.ap.password), WIFI_PASS);
    cfg.ap.ssid_len = strlen(WIFI_SSID);
    cfg.ap.channel = WIFI_CHANNEL;
    cfg.ap.authmode = WIFI_AUTH_WPA2_PSK;
    cfg.ap.sae_pwe_h2e = WPA3_SAE_PWE_BOTH;
    cfg.ap.max_connection = WIFI_MAX_STA_CONN;
    cfg.ap.pmf_cfg.required = true;
#else
    strcpy(reinterpret_cast<char *>(cfg.sta.ssid), WIFI_SSID);
    strcpy(reinterpret_cast<char *>(cfg.sta.password), WIFI_PASS);
#endif

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESPNOW_WIFI_IF, &cfg));
}

} // namespace kopter