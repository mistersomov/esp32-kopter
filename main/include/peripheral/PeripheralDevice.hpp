/*
 Copyright 2025 Ivan Somov

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

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