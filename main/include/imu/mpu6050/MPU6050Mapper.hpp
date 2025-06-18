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

#include "IIMUValueMapper.hpp"

namespace kopter {

/**
 * @enum AccelSensivityMode
 * @brief Represents accelerometer sensitivity settings for the MPU6050.
 *
 * The higher the G-force range, the less sensitive the readings (but more tolerant to higher accelerations).
 */
enum class AccelSensivityMode {
    TWO_G,
    FOUR_G,
    EIGHT_G,
    SIXTEEN_G
};

/**
 * @enum GyroSensivityMode
 * @brief Represents gyroscope sensitivity settings for the MPU6050.
 *
 * Typically maps to angular velocity ranges like ±250, ±500, ±1000, and ±2000 degrees/second.
 * NOTE: Naming as ZERO/ONE/TWO/THREE may be implementation-specific and not self-descriptive.
 */
enum class GyroSensivityMode {
    ZERO,
    ONE,
    TWO,
    THREE
};

/**
 * @class MPU6050Mapper
 * @brief Provides default conversion of raw IMU data to physical units for the MPU6050 sensor.
 *
 * This class implements the IIMUValueMapper interface and provides default
 * conversions for raw accelerometer and gyroscope values retrieved from the MPU6050 sensor.
 * These conversions typically depend on the sensor's sensitivity settings (not shown here),
 * but the implementation assumes default sensitivity modes unless extended.
 *
 * The mapping functions convert 16-bit signed raw sensor data into floating-point values
 * representing either acceleration (in g) or angular velocity (in degrees per second).
 */
class MPU6050Mapper : public IIMUValueMapper {
public:
    /**
     * @brief Ctor with configurable sensitivity for each axis.
     *
     * @param ax_mode Sensitivity for accelerometer X axis (default: TWO_G)
     * @param ay_mode Sensitivity for accelerometer Y axis (default: TWO_G)
     * @param az_mode Sensitivity for accelerometer Z axis (default: TWO_G)
     * @param gx_mode Sensitivity for gyroscope X axis (default: ZERO = ±250°/s)
     * @param gy_mode Sensitivity for gyroscope Y axis (default: ZERO = ±250°/s)
     * @param gz_mode Sensitivity for gyroscope Z axis (default: ZERO = ±250°/s)
     */
    explicit MPU6050Mapper(AccelSensivityMode ax_mode = AccelSensivityMode::TWO_G,
                           AccelSensivityMode ay_mode = AccelSensivityMode::TWO_G,
                           AccelSensivityMode az_mode = AccelSensivityMode::TWO_G,
                           GyroSensivityMode gx_mode = GyroSensivityMode::ZERO,
                           GyroSensivityMode gy_mode = GyroSensivityMode::ZERO,
                           GyroSensivityMode gz_mode = GyroSensivityMode::ZERO) noexcept;

    /**
     * @brief Default dtor.
     */
    ~MPU6050Mapper() override = default;

    /**
     * @brief Maps raw X-axis accelerometer value to acceleration in g.
     * @param raw Raw 16-bit value from sensor.
     * @return Acceleration in g.
     */
    float map_accel_x(int16_t raw) const override;

    /**
     * @brief Maps raw Y-axis accelerometer value to acceleration in g.
     * @param raw Raw 16-bit value from sensor.
     * @return Acceleration in g.
     */
    float map_accel_y(int16_t raw) const override;

    /**
     * @brief Maps raw Z-axis accelerometer value to acceleration in g.
     * @param raw Raw 16-bit value from sensor.
     * @return Acceleration in g.
     */
    float map_accel_z(int16_t raw) const override;

    /**
     * @brief Maps raw X-axis gyroscope value to angular velocity in degrees per second.
     * @param raw Raw 16-bit value from sensor.
     * @return Angular velocity in °/s.
     */
    float map_gyro_x(int16_t raw) const override;

    /**
     * @brief Maps raw Y-axis gyroscope value to angular velocity in degrees per second.
     * @param raw Raw 16-bit value from sensor.
     * @return Angular velocity in °/s.
     */
    float map_gyro_y(int16_t raw) const override;

    /**
     * @brief Maps raw Z-axis gyroscope value to angular velocity in degrees per second.
     * @param raw Raw 16-bit value from sensor.
     * @return Angular velocity in °/s.
     */
    float map_gyro_z(int16_t raw) const override;

private:
    /**
     * @brief Internal helper to convert raw accelerometer value using the specified mode.
     * @param value Raw value from sensor.
     * @param mode Sensitivity mode.
     * @return Acceleration in g.
     */
    float map_accel(int16_t value, AccelSensivityMode mode) const;

    /**
     * @brief Internal helper to convert raw gyroscope value using the specified mode.
     * @param value Raw value from sensor.
     * @param mode Sensitivity mode.
     * @return Angular velocity in °/s.
     */
    float map_gyro(int16_t value, GyroSensivityMode mode) const;

    AccelSensivityMode m_ax_mode;
    AccelSensivityMode m_ay_mode;
    AccelSensivityMode m_az_mode;
    GyroSensivityMode m_gx_mode;
    GyroSensivityMode m_gy_mode;
    GyroSensivityMode m_gz_mode;
};

} // namespace kopter