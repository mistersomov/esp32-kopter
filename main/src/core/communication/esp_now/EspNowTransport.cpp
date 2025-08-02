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
#include "EspNowTransport.hpp"

#include "MessageException.hpp"

#include <cstring>

namespace kopter {

namespace {
constexpr std::string_view TAG = "[EspNowTransport]";
} // namespace

EspNowTransport *EspNowTransport::s_instance = nullptr;

EspNowTransport::EspNowTransport(const std::array<uint8_t, ESP_NOW_ETH_ALEN> &mac, uint8_t channel)
    : m_dest_mac{std::move(mac)}, m_wifi_channel{channel}
{
    s_instance = this;

    try {
        check_call<KopterException>(esp_now_init());
        check_call<KopterException>(esp_now_register_recv_cb(&EspNowTransport::on_receive_cb));
        check_call<KopterException>(add_peer_to_list());
    }
    catch (const KopterException &e) {
        ESP_LOGE(TAG.data(), "Init failed: %s", e.what());
    }
}

EspNowTransport::~EspNowTransport()
{
    s_instance = nullptr;

    try {
        check_call<KopterException>(esp_now_deinit());
    }
    catch (const KopterException &e) {
        ESP_LOGE(TAG.data(), "ESP-NOW destroying failed: %s", e.what());
    }
}

void EspNowTransport::send(const Message &message)
{
    std::array<uint8_t, Message::size()> buffer;

    message.serialize(buffer.data());
    check_call<MessageException>(esp_now_send(m_dest_mac.data(), buffer.data(), sizeof(buffer)));
}

void EspNowTransport::attach_rx_queue(QueueHandle_t rx_queue)
{
    m_rx_queue = rx_queue;
}

void IRAM_ATTR EspNowTransport::on_receive_cb(const esp_now_recv_info_t *esp_now_info,
                                              const uint8_t *data,
                                              int data_len)
{
    if (!s_instance || !s_instance->m_rx_queue || !data || data_len <= 0) {
        return;
    }

    auto msg_opt = Message::deserialize(data);
    if (!msg_opt.has_value()) {
        return;
    }

    auto *msg = new Message(std::move(msg_opt.value()));
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xQueueSendFromISR(s_instance->m_rx_queue, &msg, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

esp_err_t EspNowTransport::add_peer_to_list() const
{
    esp_now_peer_info peer_info{};
    peer_info.channel = m_wifi_channel;
    peer_info.ifidx = WIFI_IF_STA;
    peer_info.encrypt = false;
    std::copy(m_dest_mac.data(), m_dest_mac.data() + ESP_NOW_ETH_ALEN, peer_info.peer_addr);

    return esp_now_add_peer(&peer_info);
}

} // namespace kopter