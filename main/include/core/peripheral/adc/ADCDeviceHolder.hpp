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

#pragma once

#include "ADCDevice.hpp"
#include "ADCMode.hpp"

namespace kopter {

class ADCDeviceHolder {
public:
    ADCDeviceHolder(const ADCDeviceHolder &) = delete;
    ADCDeviceHolder &operator=(const ADCDeviceHolder &) = delete;
    ~ADCDeviceHolder();

    static ADCDeviceHolder &get_instance();
    Device *add_device(const std::string &name, ADCMode mode, const std::unordered_set<adc_channel_t> &channels);

private:
    ADCDeviceHolder();

    void configure_continuous_driver();
    void configure_one_shot_driver();
    void configure_calibration();
#ifdef ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED
    void configure_line_fitting_calibration();
#elif ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
    void configure_curve_fitting_calibration();
#endif
    void add_device_continuous(const std::unordered_set<adc_channel_t> &channels);
    void add_device_one_shot(const std::unordered_set<adc_channel_t> &channels);
    void start_polling();
    void stop_polling();

    adc_continuous_handle_t m_continuous_handler{nullptr};
    adc_oneshot_unit_handle_t m_one_shot_handler{nullptr};
    adc_cali_handle_t m_cali_handler{nullptr};
    std::unordered_map<std::string, std::unique_ptr<Device>> m_devices;
};

} // namespace kopter