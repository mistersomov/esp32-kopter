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
#include "CommunicationService.hpp"

#include "MessageException.hpp"

namespace kopter {

namespace {
constexpr uint8_t RX_QUEUE_SIZE = 6;
constexpr uint16_t RX_MESSAGE_TASK_STACK_SIZE = 4096;
constexpr std::string_view RX_MESSAGE_TASK_NAME = "msg_task";
constexpr std::string_view TAG = "[MessageManager]";
} // namespace

CommunicationService::CommunicationService(std::unique_ptr<IMessageTransport> transport) noexcept
    : m_transport{std::move(transport)}, m_rx_queue{nullptr}
{
    m_rx_queue = xQueueCreate(RX_QUEUE_SIZE, sizeof(Message *));
    if (m_rx_queue == nullptr) {
        ESP_LOGE(TAG.data(), "Failed to create messages queue.");
        return;
    }
    // Always call attach_rx_queue, relies on default no-op in transports that don't use it
    m_transport->attach_rx_queue(m_rx_queue);
    create_rx_task();
}

CommunicationService::~CommunicationService()
{
    if (m_rx_queue) {
        vQueueDelete(m_rx_queue);
        m_rx_queue = nullptr;
    }
}

void CommunicationService::send_message(const Message &msg) const
{
    try {
        m_transport->send(msg);
    }
    catch (const MessageException &e) {
        ESP_LOGE(TAG.data(), "Failed to send message: %s", e.what());
    }
}

void CommunicationService::set_rx_callback(RxCallback rx_cb) noexcept
{
    m_rx_cb = rx_cb;
}

void CommunicationService::create_rx_task()
{
    m_rx_task = std::make_unique<Task>(RX_MESSAGE_TASK_NAME.data(), RX_MESSAGE_TASK_STACK_SIZE, [this]() {
        Message *msg_ptr = nullptr;

        while (true) {
            if (xQueueReceive(m_rx_queue, &msg_ptr, portMAX_DELAY) == pdTRUE) {
                if (msg_ptr && m_rx_cb) {
                    m_rx_cb(*msg_ptr);
                    delete msg_ptr;
                }
            }
        }
    });
}

} // namespace kopter