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

#include "IMUData.hpp"

#include <glm/gtc/quaternion.hpp>

namespace kopter {

/**
 * @interface IOrientationFilter
 * @brief Interface for orientation estimation filters using IMU sensor data.
 *
 * Defines the basic operations required for any orientation filter that estimates device orientation
 * using gyroscope and accelerometer data. Implementations should provide a way to update the filter
 * state with new sensor data and retrieve the current orientation as a quaternion.
 */
struct IOrientationFilter {
    /**
     * @brief Virtual dtor for interface.
     *
     * Ensures proper cleanup of derived classes when deleted via a base class pointer.
     */
    virtual ~IOrientationFilter() = default;

    /**
     * @brief Returns the current estimated orientation as a quaternion.
     *
     * @return A constant reference to a `glm::quat` representing the orientation.
     */
    virtual const glm::quat &get_quat() const noexcept = 0;

    /**
     * @brief Updates the orientation filter with new sensor data.
     *
     * @param data IMUData structure containing angular velocities and linear accelerations.
     * @param timestamp_us Timestamp of the current measurement in microseconds.
     */
    virtual void update(const IMUData &data, int64_t timestamp_us) = 0;
};

} // namespace kopter
