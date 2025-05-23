#ifndef MESSAGE_MANAGER_HPP
#define MESSAGE_MANAGER_HPP

#include "peripheral/DeviceID.hpp"

#include "esp_now.h"

#include <unordered_map>

namespace kopter {

class Message;
class Task;

class MessageManager {
public:
    using recv_callback = std::function<void(const Message &msg)>;

    MessageManager() = default;
    MessageManager(const MessageManager &) = delete;
    MessageManager &operator=(const MessageManager &) = delete;
    ~MessageManager();

    static MessageManager &get_instance();

    void init();
    void register_callback(DeviceID id, recv_callback cb);
    void send_message(const Message &msg) const;

private:
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