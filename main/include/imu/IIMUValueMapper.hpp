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
 * @interface IIMUValueMapper
 * @brief Interface for mapping raw IMU sensor values to physical units.
 *
 * This interface defines how to convert raw values from an IMU sensor's
 * accelerometer and gyroscope into meaningful physical units — acceleration in g
 * and angular velocity in degrees per second.
 *
 * Implementations may apply scaling, calibration, or filtering depending on sensor
 * sensitivity or configuration.
 *
 * Usage examples include sensor drivers or sensor fusion components that need
 * normalized physical values rather than raw binary data.
 */
struct IIMUValueMapper {
    virtual ~IIMUValueMapper() = default;

    /**
     * @brief Maps raw accelerometer value on the X axis to physical acceleration (g).
     *
     * @param raw 16-bit signed raw value from the sensor.
     * @return Acceleration on the X axis in g units.
     */
    virtual float map_accel_x(int16_t raw) const = 0;

    /**
     * @brief Maps raw accelerometer value on the Y axis to physical acceleration (g).
     *
     * @param raw 16-bit signed raw value from the sensor.
     * @return Acceleration on the Y axis in g units.
     */
    virtual float map_accel_y(int16_t raw) const = 0;

    /**
     * @brief Maps raw accelerometer value on the Z axis to physical acceleration (g).
     *
     * @param raw 16-bit signed raw value from the sensor.
     * @return Acceleration on the Z axis in g units.
     */
    virtual float map_accel_z(int16_t raw) const = 0;

    /**
     * @brief Maps raw gyroscope value on the X axis to angular velocity (°/s).
     *
     * @param raw 16-bit signed raw value from the sensor.
     * @return Angular velocity on the X axis in degrees per second.
     */
    virtual float map_gyro_x(int16_t raw) const = 0;

    /**
     * @brief Maps raw gyroscope value on the Y axis to angular velocity (°/s).
     *
     * @param raw 16-bit signed raw value from the sensor.
     * @return Angular velocity on the Y axis in degrees per second.
     */
    virtual float map_gyro_y(int16_t raw) const = 0;

    /**
     * @brief Maps raw gyroscope value on the Z axis to angular velocity (°/s).
     *
     * @param raw 16-bit signed raw value from the sensor.
     * @return Angular velocity on the Z axis in degrees per second.
     */
    virtual float map_gyro_z(int16_t raw) const = 0;
};

} // namespace kopter