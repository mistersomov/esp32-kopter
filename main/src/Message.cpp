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

#include <cstring>

namespace kopter {

Message Message::deserialize(const uint8_t *buffer)
{
    assert(buffer);

    Message message;
    std::memcpy(message.device_tag, buffer, sizeof(message.device_tag));
    std::memcpy(&message.data, buffer + sizeof(message.device_tag), sizeof(message.data));

    return message;
}

void Message::serialize(uint8_t *buffer) const
{
    assert(buffer);

    std::memcpy(buffer, device_tag, sizeof(device_tag));
    std::memcpy(buffer + sizeof(device_tag), &data, sizeof(data));
}

} // namespace kopter