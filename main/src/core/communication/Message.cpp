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
#include "Message.hpp"

#include "ByteUtils.hpp"
#include "CRCUtils.hpp"

#include <cstring>

namespace kopter {

namespace {
constexpr uint8_t PAYLOAD_SIZE = 5;

// Offsets for serialization layout
constexpr size_t OFFSET_TYPE = 0;
constexpr size_t OFFSET_THROTTLE = 1;
constexpr size_t OFFSET_ROLL = 2;
constexpr size_t OFFSET_PITCH = 3;
constexpr size_t OFFSET_YAW = 4;
constexpr size_t OFFSET_CRC = 5;

constexpr std::string_view TAG = "[Message]";
} // namespace

std::optional<Message> Message::deserialize(const uint8_t *buffer)
{
    Message message{};
    message.type = static_cast<MessageType>(ByteUtils::read_u16_le(buffer + OFFSET_TYPE));
    message.throttle = buffer[OFFSET_THROTTLE];
    message.roll = static_cast<int8_t>(buffer[OFFSET_ROLL]);
    message.pitch = static_cast<int8_t>(buffer[OFFSET_PITCH]);
    message.yaw = static_cast<int8_t>(buffer[OFFSET_YAW]);
    message.crc = ByteUtils::read_u16_le(buffer + OFFSET_CRC);

    uint16_t actual_crc = CRCUtils::crc16_ccitt(buffer, PAYLOAD_SIZE);
    if (actual_crc != message.crc) {
        ESP_LOGE(TAG.data(), "CRC mismatch: expected 0x%04X, computed 0x%04X", message.crc, actual_crc);
        return std::nullopt;
    }

    return message;
}

void Message::serialize(uint8_t *buffer) const
{
    buffer[OFFSET_TYPE] = static_cast<uint8_t>(type);
    buffer[OFFSET_THROTTLE] = throttle;
    buffer[OFFSET_ROLL] = static_cast<uint8_t>(roll);
    buffer[OFFSET_PITCH] = static_cast<uint8_t>(pitch);
    buffer[OFFSET_YAW] = static_cast<uint8_t>(yaw);

    uint16_t computed_crc = CRCUtils::crc16_ccitt(buffer, PAYLOAD_SIZE);
    ByteUtils::write_u16_le(computed_crc, buffer + OFFSET_CRC);
}

} // namespace kopter