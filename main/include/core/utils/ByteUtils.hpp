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

#include <cstdint>

namespace kopter {

struct ByteUtils {
    static constexpr uint16_t get_u16(uint8_t l, uint8_t h)
    {
        return static_cast<uint16_t>(h) << 8 | l;
    }

    static constexpr int16_t get_s16(uint8_t l, uint8_t h)
    {
        return static_cast<int16_t>(h) << 8 | l;
    }
};

} // namespace kopter