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
#include "ContinuousReadStrategy.hpp"

#include "AdcException.hpp"

namespace kopter {

static constexpr uint32_t TIMEOUT = 0;

ContinuousReadStrategy::ContinuousReadStrategy(adc_continuous_handle_t shared_handler,
                                               adc_cali_handle_t shared_cali_handler,
                                               const size_t &frame_size)
    : m_continuous_handler{shared_handler}, m_cali_handler{shared_cali_handler}, m_frame_size{frame_size}
{
}

void ContinuousReadStrategy::read(reading_callback cb)
{
    std::vector<uint8_t> buf(m_frame_size);
    uint32_t out_length = 0;

    ADC_CHECK_THROW(adc_continuous_read(m_continuous_handler, buf.data(), m_frame_size, &out_length, TIMEOUT));

    if (out_length > 0) {
        size_t sample_count = out_length / sizeof(adc_digi_output_data_t);
        const auto *samples = reinterpret_cast<adc_digi_output_data_t *>(buf.data());

        for (auto i = 0; i != sample_count; ++i) {
            const auto channel = static_cast<adc_channel_t>(samples[i].type1.channel);
            const auto raw_value = samples[i].type1.data;

            if (m_cali_handler) {
                auto voltage = 0;

                adc_cali_raw_to_voltage(m_cali_handler, raw_value, &voltage);
                cb(voltage, channel);
            }
            else {
                cb(raw_value, channel);
            }
        }
    }
}

} // namespace kopter