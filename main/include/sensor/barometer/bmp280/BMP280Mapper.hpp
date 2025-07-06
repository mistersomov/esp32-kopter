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

#include "BMP280Calibration.hpp"

namespace kopter {

/**
 * @brief Mapper for BMP280 readings.
 */
class BMP280Mapper {
public:
    /**
     * @brief Ctor for a BMP280 Mapper.
     */
    BMP280Mapper();

    /**
     * @brief Maps the raw temperature value.
     *
     * @param raw 32-bit signed raw value from the sensor.
     * @param calib Pointer to the BMP280 calibration data.
     * @return Transformed temperature in degrees Celsius.
     */
    float map_temperature(int32_t raw, BMP280Calibration *calib);

    /**
     * @brief Maps the raw pressure value.
     *
     * @param raw 32-bit unsigned raw value from the sensor.
     * @param calib Pointer to the BMP280 calibration data.
     * @return Transformed pressure in Pascals (Pa).
     */
    float map_pressure(uint32_t raw, BMP280Calibration *calib);

    /**
     * @brief Maps the calculated pressure value into altitude.
     *
     * @param raw 32-bit unsigned raw value from the sensor.
     * @return Transformed pressure in Pascals (Pa).
     */
    float map_altitude(float pressure);

private:
    /**
     * @brief Compensates the raw temperature reading using the calibration data.
     *
     * This method applies the BMP280 temperature compensation algorithm
     * and stores an intermediate fine temperature value used for pressure calculation.
     *
     * @param adc_t Raw temperature value from the BMP280 sensor (signed 32-bit integer).
     * @param calib Pointer to the BMP280 calibration data.
     * @return Compensated temperature in degrees Celsius.
     */
    float get_compensated_temperature(int32_t adc_t, BMP280Calibration *calib);

    /**
     * @brief Compensates the raw pressure reading using the calibration data.
     *
     * This method applies the BMP280 pressure compensation algorithm.
     * It uses the internally stored fine temperature value `m_t_fine`
     * from the last temperature compensation.
     *
     * @param adc_p Raw pressure value from the BMP280 sensor (unsigned 32-bit integer).
     * @param calib Pointer to the BMP280 calibration data.
     * @return Compensated pressure in Pascals (Pa).
     */
    float get_compensated_pressure(uint32_t adc_p, BMP280Calibration *calib);

    /**
     * @brief Intermediate temperature value used in pressure compensation.
     *
     * This value is calculated during temperature compensation and reused
     * during pressure compensation according to the BMP280 datasheet.
     */
    int32_t m_t_fine;
};

} // namespace kopter