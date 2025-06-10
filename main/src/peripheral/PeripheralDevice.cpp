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
#include "peripheral/PeripheralDevice.hpp"

namespace kopter {

static constexpr uint32_t TIMEOUT = 0;

static constexpr std::string_view TAG = "[PeripheralDevice]";

PeripheralDevice::PeripheralDevice(const size_t &frame_size, adc_continuous_handle_t shared_handler)
    : m_handler(shared_handler), m_frame_size(frame_size)
{
}

PeripheralDevice::~PeripheralDevice()
{
}

esp_err_t PeripheralDevice::read_value(uint8_t *buf, uint32_t *out_length)
{
    return adc_continuous_read(m_handler, buf, m_frame_size, out_length, TIMEOUT);
}

} // namespace kopter