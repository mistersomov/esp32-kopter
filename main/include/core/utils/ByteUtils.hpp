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

/**
 * @brief Utility functions for byte-level data manipulation.
 */
struct ByteUtils {
    /**
     * @brief Combines two bytes into an unsigned 16-bit integer (little-endian).
     */
    static constexpr uint16_t get_u16_le(uint8_t l, uint8_t h)
    {
        return static_cast<uint16_t>(h) << 8 | l;
    }

    /**
     * @brief Combines two bytes into a signed 16-bit integer (little-endian).
     */
    static constexpr int16_t get_s16_le(uint8_t l, uint8_t h)
    {
        return static_cast<int16_t>(h) << 8 | l;
    }

    /**
     * @brief Writes a 16-bit unsigned integer to a 8-bit unsigned int buffer in little-endian order.
     *
     * @param value The 16-bit value.
     * @param buffer Pointer to at least 2 bytes of memory.
     *
     * @note The buffer must not be null and must point to at least 2 bytes.
     */
    static void write_u16_le(uint16_t value, uint8_t *buffer)
    {
        buffer[0] = static_cast<uint8_t>(value & 0xFF);
        buffer[1] = static_cast<uint8_t>((value >> 8) & 0xFF);
    }

    /**
     * @brief Reads a 16-bit unsigned integer from a 8-bit unsigned int buffer in little-endian order.
     *
     * @param buffer Pointer to at least 2 bytes of memory.
     * @return The reconstructed 16-bit value.
     */
    static uint16_t read_u16_le(const uint8_t *buffer)
    {
        return static_cast<uint16_t>(buffer[1]) << 8 | buffer[0];
    }
};

} // namespace kopter