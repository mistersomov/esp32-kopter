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

#include "nvs_handle.hpp"

namespace kopter {

/**
 * @brief Service for managing firmware.
 *
 * This class provides access to read and write the firmware version number
 * stored persistently on the ESP32's flash memory using the NVS subsystem.
 *
 * It is implemented as a thread-safe singleton and handles NVS initialization internally.
 */
class FirmwareService {
public:
    FirmwareService(const FirmwareService &) = delete;
    FirmwareService &operator=(const FirmwareService &) = delete;

    /**
     * @brief Default dtor.
     */
    ~FirmwareService() = default;

    /**
     * @brief Get the singleton instance of FirmwareService.
     *
     * Initializes the instance on first call and returns the same instance afterward.
     *
     * @return Reference to the singleton FirmwareService instance.
     */
    static FirmwareService &get_instance();

    /**
     * @brief Get the currently stored firmware version.
     *
     * Reads the firmware version from NVS. If no version is found, a default version is written and returned.
     *
     * @return The firmware version number.
     * @throws FirmwareException If NVS open, read, or close fails.
     */
    uint16_t get_version();

    /**
     * @brief Set and store the firmware version.
     *
     * Writes the provided version number to NVS and commits the change.
     *
     * @param new_version The new firmware version to store.
     * @throws FirmwareException If NVS open, write, commit, or close fails.
     */
    void set_version(const uint16_t new_version);

private:
    FirmwareService();

    std::unique_ptr<nvs::NVSHandle> open_nvs();
};

} // namespace kopter