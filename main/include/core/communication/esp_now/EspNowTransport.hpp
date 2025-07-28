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

#include "IMessageTransport.hpp"

#include "esp_now.h"

namespace kopter {

/**
 * @brief ESP-NOW-based implementation of `IMessageTransport`.
 *
 * This class provides a concrete implementation of the `IMessageTransport` interface,
 * allowing messages to be sent over the ESP-NOW protocol to a predefined destination MAC address.
 *
 * The transport automatically initializes and deinitializes ESP-NOW in its constructor and destructor.
 * Peer information is configured and registered during construction.
 */
class EspNowTransport : public IMessageTransport {
public:
    /**
     * @brief Constructs the ESP-NOW transport with the target MAC address.
     *
     * This constructor initializes ESP-NOW and adds the destination MAC address to the peer list.
     *
     * @param mac The MAC address of the remote peer (6 bytes).
     */
    explicit EspNowTransport(std::array<uint8_t, ESP_NOW_ETH_ALEN> mac);

    /**
     * @brief Dtor for EspNowTransport.
     *
     * Automatically deinitializes ESP-NOW. Errors during deinitialization are logged.
     */
    ~EspNowTransport() override;

    /**
     * @brief Sends a message to the remote peer using ESP-NOW.
     *
     * The message is serialized into a byte buffer and transmitted using the ESP-NOW protocol
     * to the MAC address specified during construction.
     *
     * @param message The message to be sent.
     *
     * @throws MessageException if sending fails.
     */
    void send(const Message &message) override;

private:
    /**
     * @brief Adds the destination MAC address as a peer to the ESP-NOW peer list.
     *
     * This method sets the communication channel, interface index and peer MAC address,
     * and registers the peer with ESP-NOW.
     *
     * @return ESP_OK on success, or an error code from esp_err_t.
     */
    [[nodiscard]] esp_err_t add_peer_to_list() const;

    std::array<uint8_t, ESP_NOW_ETH_ALEN> m_dest_mac;
};

} // namespace kopter