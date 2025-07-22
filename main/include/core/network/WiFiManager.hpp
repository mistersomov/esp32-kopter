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

#pragma once

#include "WiFiEvent.hpp"

#include "esp_netif.h"

namespace kopter {

class EventService;

/**
 * @brief Singleton class for managing Wi-Fi in AP+STA mode using ESP-IDF.
 *
 * This class initializes the Wi-Fi stack, configures both Access Point and Station modes,
 * and handles automatic reconnection and event registration.
 */
class WiFiManager {
public:
    WiFiManager(const WiFiManager &) = delete;
    WiFiManager &operator=(const WiFiManager &) = delete;

    /**
     * @brief Dtor. Stops Wi-Fi and releases resources.
     *
     * @throws WiFiException If stopping Wi-Fi fails.
     */
    ~WiFiManager();

    /**
     * @brief Returns a singleton instance of WiFiManager.
     *
     * @param p_event_service Pointer to `EventService` used for event registration. Must not be null on first call.
     * @return `WiFiManager&` Reference to the singleton instance.
     */
    static WiFiManager &get_instance(EventService *p_event_service);

    /**
     * @brief Initializes and starts WiFi.
     *
     * Initializes NVS, network interfaces, and starts Wi-Fi in APSTA mode.
     *
     * @throws WiFiException if initialization or Wi-Fi start fails.
     */
    void init();

    /**
     * @brief Checks SSID.
     *
     * @return `true` if SSID is empty.
     */
    bool is_ssid_empty() const;

private:
    /**
     * @brief Constructs the WiFiManager with the provided `EventService`.
     *
     * @param p_event_service Pointer to the loop manager for event dispatching.
     */
    WiFiManager(EventService *p_event_service);

    /**
     * @brief Initializes Wi-Fi and sets AP + STA configuration.
     *
     * @throws WiFiException if initialization or configuration fails.
     */
    void set_wifi_config();

    /**
     * @brief Configures Wi-Fi Access Point mode.
     *
     * @throws WiFiException if AP config fails.
     */
    void set_wifi_ap_config();

    /**
     * @brief Configures Wi-Fi Station (STA) mode.
     *
     * @throws WiFiException if STA config fails.
     */
    void set_wifi_sta_config();

    /**
     * @brief Registers system Wi-Fi related events (e.g., STA start/disconnect/got IP).
     *
     * @throws WiFiException if event registration fails.
     */
    void set_event_handler();

    /**
     * @brief Initiates Wi-Fi STA connection.
     */
    void sta_do_connect();

    /**
     * @brief Attempts to reconnect to STA with retry limits.
     */
    void sta_do_reconnect();

    /**
     * @brief Cleans up when STA connection fails permanently.
     */
    void sta_do_disconnect();

    constexpr wifi_auth_mode_t get_auth_mode() const;

    EventService *m_event_service{nullptr};
    esp_netif_t *m_netif_ap{nullptr};
    esp_netif_t *m_netif_sta{nullptr};
    std::vector<std::unique_ptr<idf::event::ESPEventReg>> m_event_regs;
    uint8_t m_retry_count;
};

} // namespace kopter