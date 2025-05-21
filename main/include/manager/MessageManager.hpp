#ifndef MESSAGE_MANAGER_HPP
#define MESSAGE_MANAGER_HPP

#include "esp_now.h"
#include "freertos/FreeRTOS.h"

namespace kopter {

class Message;
class Task;

class MessageManager {
public:
    MessageManager();
    MessageManager(const MessageManager &) = delete;
    MessageManager &operator=(const MessageManager &) = delete;
    ~MessageManager();

    static MessageManager &get_instance();

    void init();
    void send_message(Message *msg) const;

private:
    static void esp_now_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status);
    static void esp_now_recv_cb_forwarder(const esp_now_recv_info_t *info, const uint8_t *data, int len);

    void handle_recv(const esp_now_recv_info_t *info, const uint8_t *data, int len);
    esp_err_t add_peer_to_list() const;

    Task *m_receive_task{nullptr};
    QueueHandle_t m_queue{nullptr};
    // Static pointer for callback
    inline static MessageManager *s_callback_instance = nullptr;
};

} // namespace kopter

#endif