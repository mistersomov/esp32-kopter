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

#include "Message.hpp"

#include <functional>

namespace kopter {

/**
 * @brief Interface for message transport layers.
 *
 * `IMessageTransport` is an abstract base class that provides an interface
 * for sending serialized messages to a target device specified by its MAC address.
 * Implementations of this interface handle the actual data transmission,
 * allowing the application logic to remain protocol-agnostic.
 */
struct IMessageTransport {
    /**
     * @brief Virtual dtor for safe cleanup of derived transport classes.
     */
    virtual ~IMessageTransport() = default;

    /**
     * @brief Sends a serialized message to a remote device.
     *
     * @param mac_addr The MAC address of the target device.
     * @param message The message to send.
     */
    virtual void send(const uint8_t *mac_addr, const Message &message) = 0;
};
} // namespace kopter