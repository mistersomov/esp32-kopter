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

#ifndef PERIPHERAL_DEVICE_FACTORY_HPP
#define PERIPHERAL_DEVICE_FACTORY_HPP

#include "DeviceAdcMode.hpp"
#include "DeviceID.hpp"
#include "PeripheralDevice.hpp"

namespace kopter {
class PeripheralDeviceFactory {
public:
    PeripheralDeviceFactory(const PeripheralDeviceFactory &) = delete;
    PeripheralDeviceFactory &operator=(const PeripheralDeviceFactory &) = delete;
    ~PeripheralDeviceFactory();

    static PeripheralDeviceFactory &get_instance()
    {
        static PeripheralDeviceFactory instance;
        return instance;
    }

    std::shared_ptr<PeripheralDevice> create_device(
        DeviceID id, DeviceAdcMode mode, std::vector<adc_channel_t> &channels
    );
    void start();
    void stop();

private:
    PeripheralDeviceFactory();

    void configure_continuous_driver();

    std::unordered_map<DeviceID, std::shared_ptr<PeripheralDevice>> m_devices;
    adc_continuous_handle_t m_continuous_handler{nullptr};
};
} // namespace kopter

#endif