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
#include "JSONParser.hpp"

namespace kopter {

namespace {
constexpr std::string_view TAG = "[JSONParser]";
} // namespace

cJSON *JSONParser::parse(const std::vector<char> &buffer)
{
    cJSON *json = cJSON_Parse(buffer.data());
    if (!json) {
        ESP_LOGE(TAG.data(), "Failed to parse JSON");
        return nullptr;
    }
    return json;
}

const cJSON *JSONParser::get_json_by_name(const cJSON *object, const std::string_view &name)
{
    cJSON *result = cJSON_GetObjectItemCaseSensitive(object, name.data());
    if (result == nullptr) {
        ESP_LOGE(TAG.data(), "Failed to get JSON by name");
        return nullptr;
    }
    return result;
}

void JSONParser::delete_json(cJSON *object) noexcept
{
    cJSON_Delete(object);
}

bool JSONParser::is_number(const cJSON *object) noexcept
{
    return cJSON_IsNumber(object);
}

bool JSONParser::is_string(const cJSON *object) noexcept
{
    return cJSON_IsString(object);
}

} // namespace kopter