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

struct [[gnu::packed]] Message {

    static constexpr size_t size()
    {
        return sizeof(device_tag) + sizeof(int16_t);
    }

    static Message deserialize(const uint8_t *buffer)
    {
        Message message;
        std::copy(buffer, buffer + sizeof(message.device_tag), reinterpret_cast<uint8_t *>(&message.device_tag));
        std::copy(buffer + sizeof(message.device_tag), buffer + size(), reinterpret_cast<uint8_t *>(&message.data));

        return message;
    }

    void serialize(uint8_t *buffer) const
    {
        std::copy(reinterpret_cast<const uint8_t *>(&device_tag),
                  reinterpret_cast<const uint8_t *>(&device_tag) + sizeof(device_tag),
                  buffer);
        std::copy(reinterpret_cast<const uint8_t *>(&data),
                  reinterpret_cast<const uint8_t *>(&data) + sizeof(data),
                  buffer + sizeof(device_tag));
    }

    std::string device_tag;
    int16_t data;
};

} // namespace kopter