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

#include "IDevice.hpp"

namespace kopter {

/**
 * @brief Interface for barometric sensors.
 *
 * This interface defines the contract for all barometer devices.
 * A barometer is expected to measure atmospheric pressure and temperature,
 * and optionally estimate altitude based on the pressure reading.
 *
 * The interface is typically implemented by drivers for specific barometric
 * sensors (e.g., BMP280, BMP388).
 */
struct IBarometer : public IDevice {
    /**
     * @brief Ctor fon an IBarometer interface.
     */
    IBarometer();

    /**
     * @brief Dtor for the IBarometer interface.
     */
    virtual ~IBarometer() = default;

    /**
     * @brief Returns the name of the barometer.
     *
     * @return A null-terminated C-style string representing the device name.
     *         The returned pointer must remain valid for the lifetime of the device.
     */
    virtual const char *get_name() const noexcept override;

    /**
     * @brief Reads the current temperature from the sensor.
     *
     * @return Temperature in degrees Celsius.
     */
    virtual float read_temperature() = 0;

    /**
     * @brief Reads the current atmospheric pressure from the sensor.
     *
     * @return Pressure in Pascals (Pa).
     */
    virtual float read_pressure() = 0;

    /**
     * @brief Estimates the current altitude using the pressure reading.
     *
     * Altitude is calculated relative to standard sea-level pressure (101325 Pa).
     *
     * @return Altitude in meters (m).
     */
    virtual float read_altitude() = 0;
};

} // namespace kopter