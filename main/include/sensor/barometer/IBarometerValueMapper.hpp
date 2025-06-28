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

namespace kopter {

/**
 * @brief Interface for transforming barometric sensor readings.
 *
 * This interface allows implementing different mapping or transformation strategies
 * for values obtained from a barometer (e.g., applying calibration offsets, unit conversions,
 * or filtering).
 *
 * Typical use cases include sensor calibration, smoothing via filters, or mapping
 * values to different reference frames or units.
 */
struct IBarometerValueMapper {
    /**
     * @brief Virtual dtor.
     */
    virtual ~IBarometerValueMapper() = default;

    /**
     * @brief Maps or transforms the raw temperature value.
     *
     * This method is expected to return a calibrated, filtered, or otherwise transformed
     * temperature value.
     *
     * @return Transformed temperature in degrees Celsius.
     */
    virtual float map_temperature() = 0;

    /**
     * @brief Maps or transforms the raw pressure value.
     *
     * This method is expected to return a calibrated, filtered, or otherwise transformed
     * atmospheric pressure value.
     *
     * @return Transformed pressure in Pascals (Pa).
     */
    virtual float map_pressure() = 0;
};

} // namespace kopter