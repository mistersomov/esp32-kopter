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

#ifndef ADC_DEVICE_HPP
#define ADC_DEVICE_HPP

#include "AdcMode.hpp"
#include "Device.hpp"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_continuous.h"
#include "esp_adc/adc_oneshot.h"

namespace kopter {

class AdcDevice : public Device {
public:
    using ReadingCallback = std::function<void(int16_t voltage)>;

    AdcDevice(const std::string &name,
              AdcMode mode,
              const size_t &frame_size,
              adc_continuous_handle_t shared_handler,
              adc_cali_handle_t shared_cali_handler);
    AdcDevice(const std::string &name,
              AdcMode mode,
              const size_t &frame_size,
              adc_oneshot_unit_handle_t shared_handler,
              adc_cali_handle_t shared_cali_handler);
    virtual ~AdcDevice();

    void read(ReadingCallback cb);

private:
    AdcMode m_mode;
    size_t m_frame_size;
    adc_continuous_handle_t m_continuous_handler{nullptr};
    adc_oneshot_unit_handle_t m_one_shot_handler{nullptr};
    adc_cali_handle_t m_cali_handler{nullptr};
};

} // namespace kopter

#endif