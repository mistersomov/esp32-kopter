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

#ifndef ADC_DEVICE_HOLDER_HPP
#define ADC_DEVICE_HOLDER_HPP

#include "AdcDevice.hpp"
#include "AdcMode.hpp"

namespace kopter {

class AdcDeviceHolder {
public:
    AdcDeviceHolder(const AdcDeviceHolder &) = delete;
    AdcDeviceHolder &operator=(const AdcDeviceHolder &) = delete;
    ~AdcDeviceHolder();

    static AdcDeviceHolder &get_instance();
    Device *add_device(const std::string &name, AdcMode mode, const std::unordered_set<adc_channel_t> &channels);

private:
    AdcDeviceHolder();

    void configure_continuous_driver();
    void configure_one_shot_driver();
    void configure_calibration();
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

#endif