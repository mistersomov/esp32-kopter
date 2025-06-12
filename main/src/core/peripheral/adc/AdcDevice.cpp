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

namespace kopter {

static constexpr uint32_t TIMEOUT = 0;

AdcDevice::AdcDevice(const std::string &name,
                     AdcMode mode,
                     const size_t &frame_size,
                     adc_continuous_handle_t shared_handler,
                     adc_cali_handle_t shared_cali_handler)
    : Device{std::move(name)},
      m_mode{std::move(mode)},
      m_frame_size{frame_size},
      m_continuous_handler{shared_handler},
      m_cali_handler{shared_cali_handler}
{
}

AdcDevice::~AdcDevice()
{
}

void AdcDevice::read(ReadingCallback cb)
{
    if (m_mode == AdcMode::CONTINUOUS) {
        std::vector<uint8_t> buf(m_frame_size);
        uint32_t out_length = 0;

        try {
            CHECK_THROW(adc_continuous_read(m_continuous_handler, buf.data(), m_frame_size, &out_length, TIMEOUT));

            if (m_cali_handler && out_length > 0) {
                size_t sample_count = out_length / sizeof(adc_digi_output_data_t);
                const auto *samples = reinterpret_cast<adc_digi_output_data_t *>(buf.data());

                for (auto i = 0; i != sample_count; ++i) {
                    const auto raw_value = samples[i].type1.data;
                    auto voltage = 0;

                    adc_cali_raw_to_voltage(m_cali_handler, raw_value, &voltage);
                    cb(voltage);
                }
            }
        }
        catch (const idf::ESPException &e) {
            ESP_LOGE(get_tag().c_str(), "ADC read failed with exception: %s (0x%X)", e.what(), e.error);
        }
    }
}

} // namespace kopter