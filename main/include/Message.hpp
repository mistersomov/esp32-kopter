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

namespace kopter {

/**
 * @struct Message
 * @brief Compact fixed-size message suitable for binary communication over UART, ESP-NOW, etc.
 *
 * Structure layout (packed, 22 bytes total):
 * - device_tag[20]: null-terminated ASCII identifier (e.g., "MTR1")
 * - data: 16-bit signed integer payload
 */
struct [[gnu::packed]] Message {

    /// Total serialized message size in bytes.
    static constexpr size_t size() noexcept
    {
        return sizeof(device_tag) + sizeof(int16_t);
    };

    /**
     * @brief Creates a Message instance from a raw byte buffer.
     * @param buffer Pointer to bytes of data.
     * @return Deserialized Message object.
     */
    static Message deserialize(const uint8_t *buffer);

    /**
     * @brief Serializes the message into a raw byte buffer.
     * @param buffer Pointer to a buffer.
     */
    void serialize(uint8_t *buffer) const;

    static constexpr size_t TAG_SIZE = 20;
    char device_tag[TAG_SIZE];
    int16_t data;
};

} // namespace kopter