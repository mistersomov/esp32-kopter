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

#include "pch.hpp"
#include "I2cDeviceHolder.hpp"
#include "I2cException.hpp"

using namespace idf;

namespace kopter {

namespace {
constexpr uint32_t SDA_PIN = CONFIG_I2C_SDA_PIN;
constexpr uint32_t SCL_PIN = CONFIG_I2C_SCL_PIN;
constexpr uint32_t FREQUENCY = 400000;
} // namespace

I2cDeviceHolder::I2cDeviceHolder()
    : m_master{
          std::make_unique<I2CMaster>(I2CNumber::I2C0(), SCL_GPIO(SCL_PIN), SDA_GPIO(SDA_PIN), Frequency(FREQUENCY))}
{
}

I2cDeviceHolder &I2cDeviceHolder::get_instance()
{
    static I2cDeviceHolder instance;
    return instance;
}

I2cDevice *I2cDeviceHolder::add_device(const std::string &name, const uint8_t &address)
{
    assert(m_master);

    if (m_devices.find(name) != m_devices.end()) {
        return m_devices[name].get();
    }

    m_devices[name] = std::make_unique<I2cDevice>(I2CAddress(std::move(address)), m_master.get());
    return m_devices[name].get();
}

} // namespace kopter
