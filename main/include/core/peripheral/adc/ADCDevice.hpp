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
#include "IDevice.hpp"

namespace kopter {

/**
 * @brief Represents a generic ADC (Analog-to-Digital Converter) device using a configurable read strategy.
 *
 * The ADCDevice class abstracts an analog input device, delegating the actual data sampling
 * to a strategy that implements the IADCReadStrategy interface. This allows flexible implementation
 * for continuous, one-shot, or custom ADC read modes.
 */
class ADCDevice : public IDevice {
public:
    /**
     * @brief Ctor for a new `ADCDevice` with a specific ADC read strategy.
     *
     * @param strategy A unique pointer to an `IADCReadStrategy` implementation used to perform ADC reads.
     */
    ADCDevice(std::unique_ptr<IADCReadStrategy> strategy);

    /**
     * @brief Destroys the ADCDevice instance and releases owned resources.
     */
    virtual ~ADCDevice() override;

    /**
     * @brief Returns the name of the ADCDevice.
     *
     * @return A null-terminated C-style string representing the device name.
     *         The returned pointer must remain valid for the lifetime of the device.
     */
    virtual const char *get_name() const noexcept override;

    /**
     * @brief Initiates an ADC read operation using the provided callback.
     *
     * The actual read behavior depends on the internal strategy implementation.
     *
     * @param cb A callback function that will receive the read float value.
     */
    void read(reading_callback cb);

private:
    std::unique_ptr<IADCReadStrategy> m_read_strategy{nullptr};
};

} // namespace kopter