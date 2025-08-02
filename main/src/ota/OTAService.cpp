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
#include "OTAService.hpp"

#include "FirmwareService.hpp"
#include "JSONParser.hpp"
#include "Task.hpp"

#include "esp_crt_bundle.h"

namespace kopter {

#define FIRMWARE_URL CONFIG_FIRMWARE_URL
#define RECV_TIMEOUT CONFIG_OTA_RECV_TIMEOUT

namespace {
constexpr uint16_t BUFFER_SIZE = 4096;
constexpr std::string_view TASK_NAME = "perform_update";
constexpr uint16_t TASK_STACK_SIZE = 8192;
constexpr std::string_view TAG = "[OTAService]";
} // namespace

OTAService::OTAService() noexcept : m_meta_info{}
{
}

OTAService &OTAService::get_instance()
{
    static OTAService instance;
    return instance;
}

void OTAService::check_and_update()
{
    check_call<OTAException>(fetch_ota_info());

    if (has_newer_version()) {
        new Task(TASK_NAME.data(), TASK_STACK_SIZE, [this]() {
            ESP_LOGI(TAG.data(), "Update available: %s", m_meta_info.url.c_str());
            perform_update();
        });
    }
    else {
        ESP_LOGI(TAG.data(), "Already up-to-date.");
    }
}

esp_err_t OTAService::fetch_ota_info()
{
    esp_http_client_config_t http_cfg{};
    http_cfg.url = FIRMWARE_URL;
    http_cfg.crt_bundle_attach = esp_crt_bundle_attach;
    http_cfg.timeout_ms = RECV_TIMEOUT;
    http_cfg.keep_alive_enable = false;
    http_cfg.buffer_size = BUFFER_SIZE;
    http_cfg.buffer_size_tx = BUFFER_SIZE;

    esp_http_client_handle_t client = esp_http_client_init(&http_cfg);
    if (!client) {
        ESP_LOGE(TAG.data(), "Failed to init HTTP client");
        return ESP_ERR_HTTP_BASE;
    }
    esp_err_t err = esp_http_client_open(client, 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG.data(), "Failed to open HTTP connection");
        return ESP_ERR_HTTP_CONNECTING;
    }
    int64_t content_length = esp_http_client_fetch_headers(client);
    if (content_length <= 0) {
        ESP_LOGE(TAG.data(), "Invalid content length");
        close_and_cleanup_http_client(client);
        return ESP_ERR_INVALID_SIZE;
    }
    std::vector<char> buffer(content_length + 1);
    int read_len = esp_http_client_read(client, buffer.data(), content_length);
    if (read_len < 0) {
        ESP_LOGE(TAG.data(), "Read failed");
        close_and_cleanup_http_client(client);
        return ESP_ERR_INVALID_SIZE;
    }
    buffer[read_len] = '\0';

    close_and_cleanup_http_client(client);
    fill_meta_info(buffer);

    return ESP_OK;
}

esp_err_t OTAService::fill_meta_info(const std::vector<char> &buffer)
{
    auto json = JSONParser::parse(buffer);
    if (json == nullptr) {
        return ESP_ERR_INVALID_STATE;
    }
    auto version = JSONParser::get_json_by_name(json, "version");
    auto url = JSONParser::get_json_by_name(json, "url");

    if (!JSONParser::is_number(version) && JSONParser::is_string(url) && (url->valuestring != nullptr)))
        {
            ESP_LOGE(TAG.data(), "Invalid JSON structure. Content: %s", buffer.data());
            JSONParser::delete_json(json);
            return ESP_ERR_INVALID_RESPONSE;
        }

    m_meta_info.url = std::string(url->valuestring);
    m_meta_info.version = static_cast<uint16_t>(version->valueint);

    JSONParser::delete_json(json);

    return ESP_OK;
}

bool OTAService::has_newer_version() const
{
    return m_meta_info.version > FirmwareService::get_instance().get_version();
}

void OTAService::perform_update() const
{
    esp_http_client_config_t http_cfg{};
    http_cfg.url = m_meta_info.url.c_str();
    http_cfg.crt_bundle_attach = esp_crt_bundle_attach;
    http_cfg.timeout_ms = RECV_TIMEOUT;
    http_cfg.buffer_size = BUFFER_SIZE;
    http_cfg.keep_alive_enable = true;
    http_cfg.disable_auto_redirect = true;
    http_cfg.transport_type = HTTP_TRANSPORT_OVER_SSL;

    esp_https_ota_config_t ota_cfg{};
    ota_cfg.http_config = &http_cfg;

    esp_err_t ret = esp_https_ota(&ota_cfg);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG.data(), "OTA failed: %s", esp_err_to_name(ret));
        throw OTAException(ESP_ERR_NOT_FINISHED);
    }

    ESP_LOGI(TAG.data(), "OTA successful, restarting...");
    FirmwareService::get_instance().set_version(m_meta_info.version);
    esp_restart();
}

void OTAService::close_and_cleanup_http_client(esp_http_client_handle_t client) const
{
    check_call<OTAException>(esp_http_client_close(client));
    check_call<OTAException>(esp_http_client_cleanup(client));
}

} // namespace kopter