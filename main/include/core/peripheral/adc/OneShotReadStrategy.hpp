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
 * @brief Implements ADC reading using the ESP-IDF one-shot mode.
 *
 * This class provides synchronous reading from a predefined set of ADC channels using
 * the one-shot ADC driver. If a calibration handle is supplied, it converts raw values
 * to millivolts before delivering them through a callback.
 */
class OneShotReadStrategy : public IADCReadStrategy {
public:
    /**
     * @brief Ctor for a OneShotReadStrategy instance.
     *
     * @param shared_handler      A previously configured and valid one-shot ADC unit handle.
     * @param shared_cali_handler Optional ADC calibration handle (can be nullptr if calibration is not needed).
     * @param channels            Set of ADC channels to read during each read() call.
     */
    OneShotReadStrategy(adc_oneshot_unit_handle_t shared_handler,
                        adc_cali_handle_t shared_cali_handler,
                        const std::unordered_set<adc_channel_t> &channels);

    /**
     * @brief Default virtual dtor.
     */
    ~OneShotReadStrategy() override = default;

    /**
     * @brief Reads raw (and optionally calibrated) ADC values from configured channels.
     *
     * Iterates over the predefined ADC channels, reads the raw values using the
     * one-shot driver, and optionally converts them to voltages using the calibration
     * handle. The resulting data is passed to the provided callback.
     *
     * @param cb Callback function to handle ADC samples. Called once per channel with the sampled value.
     *
     * @throws ADCException if reading or calibration fails.
     */
    void read(reading_callback cb) override;

private:
    adc_oneshot_unit_handle_t m_one_shot_handler{nullptr};
    adc_cali_handle_t m_cali_handler{nullptr};
    std::unordered_set<adc_channel_t> m_channels;
};

} // namespace kopter
