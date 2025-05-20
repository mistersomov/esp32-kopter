#ifndef MESSAGE_MANAGER_HPP
#define MESSAGE_MANAGER_HPP

#include "esp_err.h"

namespace kopter {

class Message;

class MessageManager {
public:
    MessageManager();
    MessageManager &operator=(const MessageManager &) = delete;
    ~MessageManager();

    static MessageManager &get_instance();

    void init();
    void send_message(Message *msg) const;

private:
    esp_err_t add_broadcast_peer();

    uint8_t recv_mac[6]{0xc8, 0xf0, 0x9e, 0xb2, 0x36, 0xfd};
};

} // namespace kopter

#endif