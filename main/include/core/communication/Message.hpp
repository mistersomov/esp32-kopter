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
 * @brief Represents the type of message being transmitted.
 */
enum class MessageType : uint8_t {
    /** Message to request data (e.g. telemetry).*/
    READ,

    /** Message to send control data (e.g. motor commands).*/
    WRITE
};

/**
 * @brief Compact fixed-size message suitable for binary communication over UART, ESP-NOW, etc.
 *
 * This structure is packed and meant to be serialized into a raw byte buffer for transmission,
 * or deserialized from such buffer. It contains essential fields for drone control or telemetry exchange.
 */
struct [[gnu::packed]] Message {

    /**
     * @brief Size of the serialized message in bytes.
     */
    static constexpr size_t size()
    {
        return sizeof(MessageType) + sizeof(throttle) + sizeof(roll) + sizeof(pitch) + sizeof(yaw) + sizeof(crc);
    }

    /**
     * @brief Creates a Message instance from a raw byte buffer.
     *
     * @param buffer Pointer to bytes of data.
     * @return Deserialized Message object.
     */
    static std::optional<Message> deserialize(const uint8_t *buffer);

    /**
     * @brief Serializes the message into a raw byte buffer.
     *
     * @param buffer Pointer to a buffer.
     */
    void serialize(uint8_t *buffer) const;

    /**
     * @brief Type of the message (e.g. READ or WRITE).
     */
    MessageType type;

    /**
     * @brief Throttle value, typically mapped from user input.
     */
    uint8_t throttle;

    /**
     * @brief Roll value, e.g. left/right tilt.
     */
    int8_t roll;

    /**
     * @brief Pitch value, e.g. forward/backward tilt.
     */
    int8_t pitch;

    /**
     * @brief Yaw value, e.g. rotation around vertical axis.
     */
    int8_t yaw;

    /**
     * @brief CRC16 checksum calculated over the remaining fields of the message.
     */
    uint16_t crc;
};

} // namespace kopter