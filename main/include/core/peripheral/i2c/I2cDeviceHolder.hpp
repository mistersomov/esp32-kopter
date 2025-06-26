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

#include "I2cDevice.hpp"

namespace kopter {

class I2cDeviceHolder {
public:
    I2cDeviceHolder(const I2cDeviceHolder &) = delete;
    I2cDeviceHolder &operator=(const I2cDeviceHolder &) = delete;
    ~I2cDeviceHolder() = default;

    static I2cDeviceHolder &get_instance();
    I2cDevice *add_device(const std::string &name, const uint8_t &address);

private:
    I2cDeviceHolder();

    std::unique_ptr<idf::I2CMaster> m_master{nullptr};
    std::unordered_map<std::string, std::unique_ptr<I2cDevice>> m_devices;
};

} // namespace kopter
