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
#include "I2cDevice.hpp"

using namespace idf;

namespace kopter {

I2cDevice::I2cDevice(const std::string &name, const idf::I2CAddress &address, I2CMaster *shared_master)
    : Device(std::move(name)), m_address{std::move(address)}, m_master{shared_master}
{
}

const idf::I2CAddress &I2cDevice::get_address() const noexcept
{
    return m_address;
}

} // namespace kopter