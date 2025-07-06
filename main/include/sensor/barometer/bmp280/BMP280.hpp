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

#include "BMP280Mapper.hpp"
#include "I2cDevice.hpp"
#include "IBarometer.hpp"

namespace kopter {

/**
 * @brief Driver wrapper for Bosch BMP280 digital pressure sensor.
 *
 * This class provides a high-level C++ interface for reading pressure,
 * temperature, and altitude from a BMP280 sensor via I2C.
 *
 * Example usage:
 * ```
 * BMP280 sensor(0x76);
 * float temp = sensor.read_temperature();
 * float pressure = sensor.read_pressure();
 * float altitude = sensor.read_altitude();
 * ```
 */
class BMP280 : public IBarometer {
public:
    /**
     * @brief Ctor for a new BMP280 object.
     *
     * @param address I2C address of the BMP280 sensor.
     */
    explicit BMP280(uint8_t address);

    /**
     * @brief Dtor for the BMP280 object.
     */
    ~BMP280() override = default;

    /**
     * @brief Returns the name `"[BMP280]"`.
     *
     * @return A null-terminated C-style string representing the device name.
     *         The returned pointer must remain valid for the lifetime of the device.
     */
    const char *get_name() const noexcept override;

    /**
     * @brief Read a temperature from the sensor.
     *
     * @return float Temperature in degrees Celsius.
     */
    float read_temperature() override;

    /**
     * @brief Read a pressure from the sensor.
     *
     * @return float Pressure in pascals (Pa).
     */
    float read_pressure() override;

    /**
     * @brief Calculate altitude based on current pressure.
     *
     * Uses standard sea-level pressure (101325 Pa) for altitude calculation.
     *
     * @return float Altitude in meters.
     */
    float read_altitude() override;

private:
    void set_ctrl_meas();
    void set_config();
    void set_calib_data();

    std::unique_ptr<I2cDevice> m_i2c_device;
    std::unique_ptr<BMP280Mapper> m_mapper;
    std::unique_ptr<BMP280Calibration> m_calib;
};

} // namespace kopter