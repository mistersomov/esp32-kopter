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

#include "IADCReadStrategy.hpp"

namespace kopter {

/**
 * @brief A strategy for reading ADC data using ESP-IDF continuous (DMA-based) mode.
 *
 * This class implements the IADCReadStrategy interface and allows reading ADC frames
 * using the esp-idf continuous read API. Optionally, it supports voltage calibration
 * if a calibration handle is provided.
 *
 * Samples are read into a buffer, decoded, optionally calibrated, and passed
 * to a user-defined callback function.
 */
class ContinuousReadStrategy : public IADCReadStrategy {
public:
    /**
     * @brief Ctor for a ContinuousReadStrategy instance.
     *
     * @param shared_handler        Shared continuous ADC handle (must be previously configured and started).
     * @param shared_cali_handler   Shared ADC calibration handle, or nullptr if calibration is not required.
     * @param frame_size            Size of the buffer used to read ADC data (in bytes). Must be >= size of one DMA
     * frame.
     */
    ContinuousReadStrategy(adc_continuous_handle_t shared_handler,
                           adc_cali_handle_t shared_cali_handler,
                           size_t frame_size);

    /**
     * @brief Default virtual dtor.
     */
    ~ContinuousReadStrategy() override = default;

    /**
     * @brief Reads available ADC samples and invokes the callback for each sample.
     *
     * This method reads from the continuous ADC buffer. For each valid sample,
     * it extracts the ADC channel and raw value. If a calibration handle is present,
     * it converts the raw value into voltage (in millivolts). The result is then
     * passed to the provided callback.
     *
     * @param cb A callback function to receive ADC readings. The callback receives (value, channel),
     *           where value is either calibrated voltage (if calibration is enabled) or raw ADC value.
     *
     * @throws ADCException if reading from the ADC fails.
     */
    void read(reading_callback cb) override;

private:
    adc_continuous_handle_t m_continuous_handler;
    adc_cali_handle_t m_cali_handler;
    size_t m_frame_size;
    std::vector<uint8_t> m_buf;
};

} // namespace kopter