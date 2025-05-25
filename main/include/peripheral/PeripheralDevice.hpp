#ifndef PERIPHERAL_DEVICE_HPP
#define PERIPHERAL_DEVICE_HPP

#include "esp_adc/adc_continuous.h"

namespace kopter {

class PeripheralDevice {
public:
    PeripheralDevice(const size_t &frame_size, adc_continuous_handle_t shared_handler);
    ~PeripheralDevice();

    esp_err_t read_value(uint8_t *buf, uint32_t *out_length);

private:
    adc_continuous_handle_t m_handler;
    size_t m_frame_size;
};

} // namespace kopter

#endif