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

#include "ADCException.hpp"

namespace kopter {

#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2
#define ADC_GET_CHANNEL(result) ((result)->type1.channel)
#define ADC_GET_RAW_DATA(result) ((result)->type1.data)
#else
#define ADC_GET_CHANNEL(result) ((result)->type2.channel)
#define ADC_GET_RAW_DATA(result) ((result)->type2.data)
#endif

namespace {
constexpr uint32_t TIMEOUT = 10;
} // namespace

ContinuousReadStrategy::ContinuousReadStrategy(adc_continuous_handle_t shared_handler,
                                               adc_cali_handle_t shared_cali_handler,
                                               size_t frame_size)
    : m_continuous_handler{shared_handler}, m_cali_handler{shared_cali_handler}, m_frame_size{frame_size}
{
    m_buf.resize(frame_size);
}

void ContinuousReadStrategy::read(reading_callback cb)
{
    uint32_t out_length = 0;

    check_call<ADCException>(
        [&]() { adc_continuous_read(m_continuous_handler, m_buf.data(), m_frame_size, &out_length, TIMEOUT); });

    if (out_length > 0) {
        size_t sample_count = out_length / sizeof(adc_digi_output_data_t);
        const auto *samples = reinterpret_cast<adc_digi_output_data_t *>(m_buf.data());

        for (size_t i = 0; i != sample_count; ++i) {
            const auto channel = static_cast<adc_channel_t>(ADC_GET_CHANNEL(&samples[i]));
            const int raw_value = ADC_GET_RAW_DATA(&samples[i]);
            ADCSample sample{.channel = channel, .raw = raw_value};

            if (m_cali_handler) {
                int voltage = 0;
                check_call<ADCException>([&]() { adc_cali_raw_to_voltage(m_cali_handler, raw_value, &voltage); });
                sample.voltage = voltage;
            }
            cb(sample);
        }
    }
    else {
        throw ADCException(ESP_ERR_INVALID_SIZE);
    }
}

} // namespace kopter