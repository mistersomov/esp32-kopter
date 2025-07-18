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
 * @brief Super class for Inertial Measurement Units (IMUs).
 *
 * The IMU class defines a standard interface for sensors that provide
 * accelerometer and gyroscope data along three axes (X, Y, Z).
 * This class is intended to be subclassed by specific IMU implementations
 * (e.g., MPU6050) that handle actual communication with hardware.
 */
struct IMU : public IDevice {
    /**
     * @brief Ctor an IMU device with the given name.
     */
    IMU();

    /**
     * @brief Virtual dtor for proper cleanup of derived classes.
     */
    virtual ~IMU() override = default;

    /**
     * @brief Returns the name of the IMU.
     *
     * @return A null-terminated C-style string representing the device name.
     *         The returned pointer must remain valid for the lifetime of the device.
     */
    virtual const char *get_name() const noexcept override;

    /**
     * @brief Reads the acceleration along the X-axis in g.
     *
     * @return Acceleration value in g.
     */
    virtual float read_ax() const = 0;

    /**
     * @brief Reads the acceleration along the Y-axis in g.
     *
     * @return Acceleration value in g.
     */
    virtual float read_ay() const = 0;

    /**
     * @brief Reads the acceleration along the Z-axis in g.
     *
     * @return Acceleration value in g.
     */
    virtual float read_az() const = 0;

    /**
     * @brief Reads the angular velocity around the X-axis in degrees/sec.
     *
     * @return Angular velocity in degrees per second.
     */
    virtual float read_gx() const = 0;

    /**
     * @brief Reads the angular velocity around the Y-axis in degrees/sec.
     *
     * @return Angular velocity in degrees per second.
     */
    virtual float read_gy() const = 0;

    /**
     * @brief Reads the angular velocity around the Z-axis in degrees/sec.
     *
     * @return Angular velocity in degrees per second.
     */
    virtual float read_gz() const = 0;
};

} // namespace kopter