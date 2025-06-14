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
#include "AdcDevice.hpp"
#include "AdcException.hpp"

namespace kopter {

AdcDevice::AdcDevice(const std::string &name, std::unique_ptr<IAdcReadStrategy> strategy)
    : Device{name}, m_read_strategy{std::move(strategy)}
{
}

AdcDevice::~AdcDevice()
{
}

void AdcDevice::read(reading_callback cb)
{
    if (!m_read_strategy) {
        ESP_LOGE(get_tag().c_str(), "Read strategy not set.");
        throw AdcException(ESP_ERR_INVALID_STATE);
    }
    m_read_strategy->read(cb);
}

} // namespace kopter