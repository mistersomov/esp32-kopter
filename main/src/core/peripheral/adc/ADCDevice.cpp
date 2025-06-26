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
#include "ADCDevice.hpp"
#include "ADCException.hpp"

namespace kopter {

ADCDevice::ADCDevice(std::unique_ptr<IADCReadStrategy> strategy) : IDevice(), m_read_strategy{std::move(strategy)}
{
}

ADCDevice::~ADCDevice()
{
}

const char *ADCDevice::get_name() const noexcept
{
    return "[ADCDevice]";
}

void ADCDevice::read(reading_callback cb)
{
    if (!m_read_strategy) {
        ESP_LOGE(get_name(), "Read strategy not set.");
        throw ADCException(ESP_ERR_INVALID_STATE);
    }
    m_read_strategy->read(cb);
}

} // namespace kopter