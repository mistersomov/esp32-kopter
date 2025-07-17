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
#include "MessageManager.hpp"

#include "Message.hpp"
#include "Task.hpp"

namespace kopter {

#define MSG_CHECK_THROW(err) CHECK_THROW_WITH(err, MessageException)

#define WIFI_CHANNEL CONFIG_WIFI_AP_CHANNEL
#if CONFIG_WIFI_MODE_SOFTAP
#define ESPNOW_WIFI_IF WIFI_IF_AP
#else
#define ESPNOW_WIFI_IF WIFI_IF_STA
#endif

// Customize region begin
#if CONFIG_WIFI_MODE_SOFTAP
static uint8_t dest_mac[ESP_NOW_ETH_ALEN] = {0x2c, 0xbc, 0xbb, 0x92, 0xf7, 0xa8};
#else
static uint8_t dest_mac[ESP_NOW_ETH_ALEN] = {0xc8, 0xf0, 0x9e, 0xb2, 0x36, 0xfd};
#endif
// Customize region end

namespace {
constexpr uint8_t MESSAGE_QUEUE_SIZE = 6;
constexpr std::string_view RECV_MESSAGE_TASK_NAME = "msg_task";
constexpr uint16_t RECV_MESSAGE_TASK_STACK_SIZE = 4096;
constexpr std::string_view TAG = "[MessageManager]";
} // namespace

MessageException::MessageException(esp_err_t error) : KopterException(error)
{
}

MessageManager::MessageManager()
{
    m_msg_queue = xQueueCreate(MESSAGE_QUEUE_SIZE, sizeof(Message *));
    if (m_msg_queue == nullptr) {
        ESP_LOGE(TAG.data(), "Creating messages queue fail");
        return;
    }

    m_callback_instance = this;

    MSG_CHECK_THROW(esp_now_init());
    MSG_CHECK_THROW(esp_now_register_send_cb(esp_now_send_cb));
    MSG_CHECK_THROW(esp_now_register_recv_cb(esp_now_recv_cb_forwarder));
    MSG_CHECK_THROW(add_peer_to_list());
    create_msg_receive_task();
}

MessageManager::~MessageManager()
{
    if (m_recv_task) {
        delete m_recv_task;
        m_recv_task = nullptr;
    }
    if (m_msg_queue) {
        vQueueDelete(m_msg_queue);
        m_msg_queue = nullptr;
    }
    MSG_CHECK_THROW(esp_now_deinit());
}

MessageManager &MessageManager::get_instance()
{
    static MessageManager instance;
    return instance;
}

void MessageManager::register_callback(const std::string &device_tag, recv_callback cb)
{
    if (m_recv_callbacks.find(device_tag) == m_recv_callbacks.end()) {
        m_recv_callbacks[device_tag] = std::move(cb);
    }
}

void MessageManager::send_message(const Message &msg) const
{
    std::array<uint8_t, Message::size()> buffer;
    esp_err_t ret;

    msg.serialize(buffer.data());
    ret = esp_now_send(dest_mac, buffer.data(), sizeof(buffer));
    if (ret != ESP_OK) {
        ESP_LOGE(TAG.data(), "Failed to sending message: %s", esp_err_to_name(ret));
    }
}

IRAM_ATTR void MessageManager::esp_now_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status)
{
}

IRAM_ATTR void MessageManager::esp_now_recv_cb_forwarder(const esp_now_recv_info_t *info, const uint8_t *data, int len)
{
    if (MessageManager::m_callback_instance) {
        MessageManager::m_callback_instance->handle_recv(info, data, len);
    }
}

void MessageManager::handle_recv(const esp_now_recv_info_t *info, const uint8_t *data, int len)
{
    if (len < Message::size()) {
        ESP_LOGE(TAG.data(), "The resulting data is of incorrect length");
        return;
    }

    Message *msg = new Message(Message::deserialize(data));
    BaseType_t task_woken = pdFALSE;

    xQueueSendFromISR(m_msg_queue, &msg, &task_woken);

    if (task_woken) {
        portYIELD_FROM_ISR();
    }
}

esp_err_t MessageManager::add_peer_to_list() const
{
    esp_now_peer_info peer_info{};
    peer_info.channel = WIFI_CHANNEL;
    peer_info.ifidx = ESPNOW_WIFI_IF;
    peer_info.encrypt = false;
    std::copy(std::begin(dest_mac), std::end(dest_mac), peer_info.peer_addr);

    return esp_now_add_peer(&peer_info);
}

void MessageManager::create_msg_receive_task()
{
    m_recv_task = new Task(RECV_MESSAGE_TASK_NAME.data(), RECV_MESSAGE_TASK_STACK_SIZE, [this]() {
        Message *msg;

        while (true) {
            if (xQueueReceive(m_msg_queue, &msg, portMAX_DELAY)) {
                auto itt = m_recv_callbacks.find(msg->device_tag);
                if (itt != m_recv_callbacks.end()) {
                    itt->second(*msg);
                }
            }
            delete msg;
        }
    });
}

} // namespace kopter