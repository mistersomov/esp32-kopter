#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <array>
#include <cstdint>
#include <cstring>

namespace kopter {

__attribute__((packed)) struct Message {
    int16_t data;
    std::array<uint8_t, 6> dest_mac;

    static constexpr size_t size()
    {
        return sizeof(int16_t) + 6;
    }

    static Message deserialize(const uint16_t *buffer)
    {
        Message message;
        std::memcpy(&message.data, buffer, sizeof(&message.data));
        std::memcpy(&message.dest_mac, buffer + sizeof(message.data), message.dest_mac.size());
    }

    void serialize(uint16_t *buffer) const
    {
        std::memcpy(buffer, &data, sizeof(data));
        std::memcpy(buffer + sizeof(data), dest_mac.data(), dest_mac.size());
    }
};

} // namespace kopter

#endif