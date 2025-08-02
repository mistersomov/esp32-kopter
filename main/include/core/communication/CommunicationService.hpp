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
#include "Task.hpp"

#include "freertos/queue.h"

namespace kopter {

/**
 * @brief Central service responsible for message communication.
 *
 * Manages message sending and receiving. Owns the FreeRTOS queue and background task
 * that listens for incoming messages from the transport layer.
 */
class CommunicationService {
public:
    using RxCallback = std::function<void(const Message &msg)>;

    /**
     * @brief Constructs the communication service and starts the RX task.
     *
     * The constructor creates a FreeRTOS queue, calls attach_rx_queue on the transport (if implemented),
     * and starts a background task for receiving messages.
     *
     * @param transport  Transport implementation for message I/O.
     */
    explicit CommunicationService(std::unique_ptr<IMessageTransport> transport) noexcept;
    ~CommunicationService();

    /**
     * @brief Sends a message using the transport.
     * @param msg Message to send.
     */
    void send_message(const Message &msg) const;

    void set_rx_callback(RxCallback rx_cb) noexcept;

private:
    /**
     * @brief Creates and starts the task that listens for incoming messages.
     */
    void create_rx_task();

    std::unique_ptr<IMessageTransport> m_transport;
    std::unique_ptr<Task> m_rx_task;
    QueueHandle_t m_rx_queue;
    RxCallback m_rx_cb;
};

} // namespace kopter