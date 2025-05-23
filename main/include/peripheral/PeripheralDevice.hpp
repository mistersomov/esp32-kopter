#ifndef PERIPHERAL_DEVICE_HPP
#define PERIPHERAL_DEVICE_HPP

#include "DeviceAdcMode.hpp"

#include "esp_adc/adc_continuous.h"

namespace kopter {

class PeripheralDevice {
public:
    PeripheralDevice(DeviceAdcMode mode, adc_channel_t channel);
    ~PeripheralDevice();

    esp_err_t read_value(uint8_t *buf, uint32_t *out_length);

private:
    void continuous_adc_init();

    adc_continuous_handle_t m_handle{nullptr};
    adc_channel_t m_channel;
};

} // namespace kopter

#endif