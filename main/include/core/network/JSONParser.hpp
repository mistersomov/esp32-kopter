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

#include "cJSON.h"

namespace kopter {

/**
 * @brief A lightweight utility class for parsing and extracting data from JSON buffers using cJSON.
 */
struct JSONParser {
    /**
     * @brief Parses a JSON buffer into a cJSON object.
     *
     * @param buffer The input character buffer containing the JSON data.
     * @return A pointer to a `cJSON` object if successful, nullptr otherwise. Caller is responsible for deleting it
     * using `cJSON_Delete()`.
     */
    static cJSON *parse(const std::vector<char> &buffer);

    /**
     * @brief Retrieves a named child object from a JSON object.
     *
     * @param object The parent `cJSON` object.
     * @param name The key to search for (case-sensitive).
     * @return A pointer to the corresponding `cJSON` item, or `nullptr` if not found.
     */
    static const cJSON *get_json_by_name(const cJSON *object, const std::string_view &name);

    /**
     * @brief Deletes a JSON object.
     *
     * @param object JSON object.
     */
    static void delete_json(cJSON *object) noexcept;
};
} // namespace kopter