#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <array>
#include <cstdint>
#include <cstring>

namespace kopter {

__attribute__((packed)) struct Message {
    int16_t data;

    static constexpr size_t size()
    {
        return sizeof(int16_t);
    }

    static Message deserialize(const uint8_t *buffer)
    {
        Message message;
        std::memcpy(&message.data, buffer, sizeof(message.data));

        return message;
    }

    void serialize(uint8_t *buffer) const
    {
        std::memcpy(buffer, &data, sizeof(data));
    }
};

} // namespace kopter

#endif