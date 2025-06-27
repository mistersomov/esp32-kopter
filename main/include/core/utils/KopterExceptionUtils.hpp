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

#include "KopterException.hpp"

#include <type_traits>

namespace kopter {

#define CHECK_THROW_WITH(error, exception_type)                                                                        \
    do {                                                                                                               \
        esp_err_t result = (error);                                                                                    \
        if (result != ESP_OK)                                                                                          \
            throw kopter::exception_type(result);                                                                      \
    } while (0)

template <typename Exception,
          typename Func,
          typename = std::enable_if_t<std::is_base_of<KopterException, Exception>::value>>
inline auto check_call(Func func) -> decltype(func())
{
    try {
        return func();
    }
    catch (const idf::ESPException &e) {
        throw Exception(e.error);
    }
}

} // namespace kopter