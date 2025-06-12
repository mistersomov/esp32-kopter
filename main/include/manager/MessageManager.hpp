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

#ifndef MESSAGE_MANAGER_HPP
#define MESSAGE_MANAGER_HPP

#include "DeviceID.hpp"

#include "esp_now.h"

#include <unordered_map>

namespace kopter {

class Message;
class Task;

class MessageManager {
public:
    using recv_callback = std::function<void(const Message &msg)>;

    MessageManager(const MessageManager &) = delete;
    MessageManager &operator=(const MessageManager &) = delete;
    ~MessageManager();

    static MessageManager &get_instance();

    void register_callback(DeviceID id, recv_callback cb);
    void send_message(const Message &msg) const;

private:
    MessageManager();

    IRAM_ATTR static void esp_now_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status);
    IRAM_ATTR static void esp_now_recv_cb_forwarder(const esp_now_recv_info_t *info, const uint8_t *data, int len);

    void handle_recv(const esp_now_recv_info_t *info, const uint8_t *data, int len);
    esp_err_t add_peer_to_list() const;
    void create_msg_receive_task();

    std::unordered_map<DeviceID, recv_callback> m_recv_callbacks;
    Task *m_recv_task{nullptr};
    QueueHandle_t m_msg_queue{nullptr};
    // Static pointer for callback
    inline static MessageManager *m_callback_instance = nullptr;
};

} // namespace kopter

#endif