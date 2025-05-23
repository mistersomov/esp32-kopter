#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "peripheral/DeviceID.hpp"

namespace kopter {

struct [[gnu::packed]] Message {
    DeviceID device_id;
    int16_t data;

    static constexpr size_t size()
    {
        return sizeof(device_id) + sizeof(int16_t);
    }

    static Message deserialize(const uint8_t *buffer)
    {
        Message message;
        std::copy(buffer, buffer + sizeof(message.device_id), reinterpret_cast<uint8_t *>(&message.device_id));
        std::copy(buffer + sizeof(message.device_id), buffer + size(), reinterpret_cast<uint8_t *>(&message.data));

        return message;
    }

    void serialize(uint8_t *buffer) const
    {
        std::copy(
            reinterpret_cast<const uint8_t *>(&device_id),
            reinterpret_cast<const uint8_t *>(&device_id) + sizeof(device_id),
            buffer
        );
        std::copy(
            reinterpret_cast<const uint8_t *>(&data),
            reinterpret_cast<const uint8_t *>(&data) + sizeof(data),
            buffer + sizeof(device_id)
        );
    }
};

} // namespace kopter

#endif