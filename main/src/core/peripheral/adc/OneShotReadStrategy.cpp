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
#include "OneShotReadStrategy.hpp"

#include "ADCException.hpp"

namespace kopter {

OneShotReadStrategy::OneShotReadStrategy(adc_oneshot_unit_handle_t shared_handler,
                                         adc_cali_handle_t shared_cali_handler,
                                         const std::unordered_set<adc_channel_t> &channels)
    : m_one_shot_handler{shared_handler}, m_cali_handler{shared_cali_handler}, m_channels{channels}
{
}

void OneShotReadStrategy::read(reading_callback cb)
{
    for (const auto &chnl : m_channels) {
        int raw_value = 0;
        check_call<ADCException>([&]() { adc_oneshot_read(m_one_shot_handler, chnl, &raw_value); });
        ADCSample sample{.channel = chnl, .raw = raw_value};

        if (m_cali_handler) {
            int voltage = 0;
            check_call<ADCException>([&]() { adc_cali_raw_to_voltage(m_cali_handler, raw_value, &voltage); });
            sample.voltage = voltage;
        }
        cb(sample);
    }
}

} // namespace kopter