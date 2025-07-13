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

#include "OTAMeta.hpp"

#include "esp_https_ota.h"

namespace kopter {

/**
 * @brief A singleton service class that handles OTA update checking and application.
 */
class OTAService {
public:
    OTAService(const OTAService &) = delete;
    OTAService &operator=(const OTAService &) = delete;

    /**
     * @brief Default dtor.
     */
    ~OTAService() = default;

    /**
     * @brief Returns the singleton instance of the `OTAService`.
     *
     * @return A reference to the singleton instance.
     */
    static OTAService &get_instance();

    /**
     * @brief Checks for firmware updates and performs an OTA update if a newer version is available.
     *
     * Will download version metadata from a configured URL and compare it with the currently running firmware version.
     * If a newer version is found, the OTA process is launched on a separate task.
     *
     * @throws `OTAException` if an error occurs during metadata retrieval or parsing.
     */
    void check_and_update();

private:
    OTAService() noexcept;

    /**
     * @brief Downloads and parses the OTA metadata JSON file.
     */
    void fetch_ota_info();

    /**
     * @brief Extracts metadata from the JSON buffer.
     *
     * @param buffer A character buffer containing the JSON payload.
     */
    void fill_meta_info(const std::vector<char> &buffer);

    /**
     * @brief Compares the current firmware version with the version in the metadata.
     *
     * @return true if the metadata contains a newer version, false otherwise.
     */
    bool has_newer_version() const;

    /**
     * @brief Executes the OTA update using ESP-IDF's OTA API.
     *
     * If the update is successful, the device is restarted automatically.
     */
    void perform_update() const;

    /**
     * @brief Closes and cleans up the given HTTP client handle.
     *
     * @param client The HTTP client handle to close and clean up.
     * @throws `OTAException` if cleanup operations fail.
     */
    void close_and_cleanup_http_client(esp_http_client_handle_t client) const;

    OTAMeta m_meta_info;
};

} // namespace kopter