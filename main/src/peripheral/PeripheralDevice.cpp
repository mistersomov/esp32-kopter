#include "pch.hpp"
#include "peripheral/PeripheralDevice.hpp"

namespace kopter {

static constexpr uint32_t TIMEOUT = 0;

static constexpr std::string_view TAG = "[PeripheralDevice]";

PeripheralDevice::PeripheralDevice(const size_t &frame_size, adc_continuous_handle_t shared_handler)
    : m_handler(shared_handler), m_frame_size(frame_size)
{
}

PeripheralDevice::~PeripheralDevice()
{
}

esp_err_t PeripheralDevice::read_value(uint8_t *buf, uint32_t *out_length)
{
    return adc_continuous_read(m_handler, buf, m_frame_size, out_length, TIMEOUT);
}

} // namespace kopter