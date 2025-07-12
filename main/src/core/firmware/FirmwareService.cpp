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
#include "FirmwareService.hpp"

#include "FirmwareException.hpp"

#include "nvs_flash.h"

using namespace nvs;

namespace kopter {

inline static constexpr uint16_t DEFAULT_VERSION = 1;
inline static constexpr std::string_view STORAGE_NAME = "storage";
inline static constexpr std::string_view VERSION_KEY = "version";
inline static constexpr std::string_view TAG = "[FirmwareService]";

FirmwareService::FirmwareService()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        check_call<FirmwareException>([]() { nvs_flash_erase(); });
        check_call<FirmwareException>([]() { nvs_flash_init(); });
    }
}

FirmwareService &FirmwareService::get_instance()
{
    static FirmwareService instance;
    return instance;
}

uint16_t FirmwareService::get_version()
{
    auto handler = open_nvs();
    if (handler == nullptr) {
        return DEFAULT_VERSION;
    }

    uint16_t version;
    if (handler->get_item<uint16_t>(VERSION_KEY.data(), version) != ESP_OK) {
        set_version(DEFAULT_VERSION);
    }

    return version;
}

void FirmwareService::set_version(const uint16_t new_version)
{
    auto handler = open_nvs();
    if (handler == nullptr) {
        return;
    }
    if (handler->set_item<uint16_t>(VERSION_KEY.data(), new_version) != ESP_OK) {
        ESP_LOGE(TAG.data(), "Failed to save new version");
        return;
    }
    if (handler->commit() != ESP_OK) {
        ESP_LOGE(TAG.data(), "Failed to commit changes");
        return;
    }
}

std::unique_ptr<nvs::NVSHandle> FirmwareService::open_nvs()
{
    esp_err_t ret;
    auto handler = open_nvs_handle(STORAGE_NAME.data(), NVS_READWRITE, &ret);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG.data(),
                 "Failed to open NVS storage \"%s\" in %s mode (err: 0x%X - %s)",
                 STORAGE_NAME.data(),
                 "READWRITE",
                 ret,
                 esp_err_to_name(ret));
        return nullptr;
    }
    return handler;
}

} // namespace kopter