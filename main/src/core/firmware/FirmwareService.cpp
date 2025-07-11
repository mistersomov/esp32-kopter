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

#include "nvs.h"
#include "nvs_flash.h"

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
    nvs_handle_t handler;
    check_call<FirmwareException>([&handler]() { nvs_open(STORAGE_NAME.data(), NVS_READWRITE, &handler); });

    uint16_t version;
    esp_err_t ret = nvs_get_u16(handler, VERSION_KEY.data(), &version);
    if (ret != ESP_OK) {
        check_call<FirmwareException>([&handler]() { nvs_close(handler); });
        set_version(DEFAULT_VERSION);

        return DEFAULT_VERSION;
    }
    check_call<FirmwareException>([&handler]() { nvs_close(handler); });

    return version;
}

void FirmwareService::set_version(const uint16_t new_version)
{
    nvs_handle_t handler;

    check_call<FirmwareException>([&handler]() { nvs_open(STORAGE_NAME.data(), NVS_READWRITE, &handler); });
    check_call<FirmwareException>(
        [&handler, &new_version]() { nvs_set_u16(handler, VERSION_KEY.data(), new_version); });
    check_call<FirmwareException>([&handler]() { nvs_commit(handler); });
    check_call<FirmwareException>([&handler]() { nvs_close(handler); });
}

} // namespace kopter