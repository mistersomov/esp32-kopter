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

#include "IMU.hpp"

#include <experimental/propagate_const>

namespace kopter {

/**
 * @brief Represents an MPU6050 IMU sensor connected over I2C.
 *
 * This class provides access to raw accelerometer and gyroscope data
 * from the MPU6050 sensor. It uses an I2cDevice to communicate with the
 * hardware and an IIMUValueMapper to convert raw sensor values into
 * physical units (e.g., g-forces and degrees per second).
 *
 * The sensor is initialized by writing to the power management register
 * to wake it up from sleep mode.
 */
class MPU6050 : public IMU {
public:
    /**
     * @brief Ctor for a MPU6050 with the default value mapper.
     *
     * @param address The I2C address of the MPU6050 sensor.
     *
     * @throws I2cException with the corrsponding esp_err_t return value if something goes wrong
     */
    explicit MPU6050(uint8_t address);

    /**
     * @brief Default dtor.
     */
    ~MPU6050();

    /**
     * @brief Returns the `"[MPU6050]"`.
     *
     * @return A null-terminated C-style string representing the device name.
     *         The returned pointer must remain valid for the lifetime of the device.
     */
    const char *get_name() const noexcept override;

    /**
     * @brief Reads the acceleration value along the X axis.
     * @return Acceleration in g.
     *
     * @throws I2cException with the corrsponding esp_err_t return value if something goes wrong
     */
    float read_ax() const override;

    /**
     * @brief Reads the acceleration value along the Y axis.
     * @return Acceleration in g.
     *
     * @throws I2cException with the corrsponding esp_err_t return value if something goes wrong
     */
    float read_ay() const override;

    /**
     * @brief Reads the acceleration value along the Z axis.
     * @return Acceleration in g.
     *
     * @throws I2cException with the corrsponding esp_err_t return value if something goes wrong
     */
    float read_az() const override;

    /**
     * @brief Reads the gyroscope value around the X axis.
     * @return Angular velocity in degrees per second.
     *
     * @throws I2cException with the corrsponding esp_err_t return value if something goes wrong
     */
    float read_gx() const override;

    /**
     * @brief Reads the gyroscope value around the Y axis.
     * @return Angular velocity in degrees per second.
     *
     * @throws I2cException with the corrsponding esp_err_t return value if something goes wrong
     */
    float read_gy() const override;

    /**
     * @brief Reads the gyroscope value around the Z axis.
     * @return Angular velocity in degrees per second.
     *
     * @throws I2cException with the corrsponding esp_err_t return value if something goes wrong
     */
    float read_gz() const override;

private:
    class MPU6050Impl;
    std::experimental::propagate_const<std::unique_ptr<MPU6050Impl>> m_impl;
};

} // namespace kopter