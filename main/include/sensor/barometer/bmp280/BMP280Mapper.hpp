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
    BMP280Mapper();

    /**
     * @brief Maps or transforms the raw temperature value.
     *
     * This method is expected to return a calibrated, filtered, or otherwise transformed
     * temperature value.
     *
     * @param raw 32-bit signed raw value from the sensor.
     * @return Transformed temperature in degrees Celsius.
     */
    float map_temperature(int32_t raw, BMP280Calibration &calib);

    /**
     * @brief Maps or transforms the raw pressure value.
     *
     * This method is expected to return a calibrated, filtered, or otherwise transformed
     * atmospheric pressure value.
     *
     * @param raw 32-bit unsigned raw value from the sensor.
     * @return Transformed pressure in Pascals (Pa).
     */
    float map_pressure(uint32_t raw, BMP280Calibration &calib);

    float map_altitude(float pressure);

private:
    float get_compensated_temperature(int32_t adc_t, BMP280Calibration &calib);
    float get_compensated_pressure(uint32_t adc_p, BMP280Calibration &calib);

    int32_t m_t_fine;
};

} // namespace kopter