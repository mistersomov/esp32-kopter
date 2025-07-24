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

#include "I2cDevice.hpp"
#include "IMU.hpp"
#include "MPU6050Mapper.hpp"

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
     * @brief Reads and returns sensor data.
     *
     * @return An instance of IMUData with mapped acceleration and angular velocity.
     */
    IMUData get_data() override;

private:
    /**
     * @brief Reads the acceleration value along the X axis.
     * @return Acceleration in g.
     *
     * @throws I2cException with the corrsponding esp_err_t return value if something goes wrong
     */
    float read_ax() const;

    /**
     * @brief Reads the acceleration value along the Y axis.
     * @return Acceleration in g.
     *
     * @throws I2cException with the corrsponding esp_err_t return value if something goes wrong
     */
    float read_ay() const;

    /**
     * @brief Reads the acceleration value along the Z axis.
     * @return Acceleration in g.
     *
     * @throws I2cException with the corrsponding esp_err_t return value if something goes wrong
     */
    float read_az() const;

    /**
     * @brief Reads the gyroscope value around the X axis.
     * @return Angular velocity in degrees per second.
     *
     * @throws I2cException with the corrsponding esp_err_t return value if something goes wrong
     */
    float read_gx() const;

    /**
     * @brief Reads the gyroscope value around the Y axis.
     * @return Angular velocity in degrees per second.
     *
     * @throws I2cException with the corrsponding esp_err_t return value if something goes wrong
     */
    float read_gy() const;

    /**
     * @brief Reads the gyroscope value around the Z axis.
     * @return Angular velocity in degrees per second.
     *
     * @throws I2cException with the corrsponding esp_err_t return value if something goes wrong
     */
    float read_gz() const;

    /**
     * @brief Reads raw 16-bit sensor data from the specified register.
     *
     * This function reads two consecutive bytes starting from the given register address
     * over I2C and combines them into a signed 16-bit integer. This raw value is typically
     * used for internal conversion into physical units (e.g., acceleration or rotation rate).
     *
     * @param reg The starting register address to read from.
     * @return `int16_t` The raw 16-bit signed value from the sensor.
     *
     * @throws I2cException with the corrsponding esp_err_t return value if something goes wrong
     */
    int16_t get_raw_value(uint8_t reg) const;

    /**
     * @brief Sets config for device.
     *
     * Resets the device and disables sleep mode. Also sets the default sensitivity for the accelerometer and gyroscope.
     *
     * @throws I2cException with the corrsponding esp_err_t return value if something goes wrong
     */
    void set_config();

    /// I2C communication interface for MPU6050.
    std::unique_ptr<I2cDevice> m_i2c_device;

    /// Mapper to convert raw sensor values to physical units.
    std::unique_ptr<MPU6050Mapper> m_mapper;
};

} // namespace kopter