#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "CoreTypes.hpp"

#include <algorithm>
#include <cstdint>

namespace kopter {

struct [[gnu::packed]] Message {
    SensorID sensor_id;
    int16_t data;

    static constexpr size_t size()
    {
        return sizeof(sensor_id) + sizeof(int16_t);
    }

    static Message deserialize(const uint8_t *buffer)
    {
        Message message;
        std::copy(buffer, buffer + sizeof(message.sensor_id), reinterpret_cast<uint8_t *>(&message.sensor_id));
        std::copy(buffer + sizeof(message.sensor_id), buffer + size(), reinterpret_cast<uint8_t *>(&message.data));

        return message;
    }

    void serialize(uint8_t *buffer) const
    {
        std::copy(
            reinterpret_cast<const uint8_t *>(&sensor_id),
            reinterpret_cast<const uint8_t *>(&sensor_id) + sizeof(sensor_id),
            buffer
        );
        std::copy(
            reinterpret_cast<const uint8_t *>(&data),
            reinterpret_cast<const uint8_t *>(&data) + sizeof(data),
            buffer + sizeof(sensor_id)
        );
    }
};

} // namespace kopter

#endif