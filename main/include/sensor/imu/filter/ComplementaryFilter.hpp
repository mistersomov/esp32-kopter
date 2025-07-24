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

#include "IOrientationFilter.hpp"

namespace kopter {

/**
 * @class ComplementaryFilter
 * @brief A simple orientation filter using a complementary approach with gyro and accelerometer data.
 *
 * This class implements a complementary filter that fuses gyroscope and accelerometer data to estimate
 * device orientation represented as a quaternion. The filter corrects orientation drift from the gyroscope
 * using the long-term stable accelerometer reference.
 */
class ComplementaryFilter : public IOrientationFilter {
public:
    /**
     * @brief Ctor for a ComplementaryFilter with a given smoothing factor.
     *
     * @param alpha A smoothing factor in range (0.0, 1.0), where:
     *              - 1.0 = only gyro (no correction)
     *              - 0.0 = only accelerometer (no integration)
     *              Default is 0.95, meaning 95% of influence comes from the gyroscope.
     */
    explicit ComplementaryFilter(float alpha = 0.95f) noexcept;

    /**
     * @brief Returns the current orientation as a quaternion.
     *
     * @return A const reference to the current orientation quaternion.
     */
    const glm::quat &get_quat() const noexcept override;

    /**
     * @brief Updates the orientation estimate using IMU data and timestamp.
     *
     * @param data IMUData structure containing angular velocities and linear accelerations.
     * @param timestamp_us Timestamp of the current sample in microseconds.
     *
     * @note The first call initializes the filter and skips integration.
     * @note Accelerometer input must represent gravity-only for accurate correction.
     */
    void update(const IMUData &data, int64_t timestamp_us) override;

private:
    /**
     * @brief Applies gyroscope-based orientation change.
     *
     * Converts angular velocity to a quaternion delta and applies it to the internal orientation.
     *
     * @param gx Angular velocity X in degrees/sec.
     * @param gy Angular velocity Y in degrees/sec.
     * @param gz Angular velocity Z in degrees/sec.
     * @param dt Time delta since last update in seconds.
     */
    void apply_gyro_influence(float gx, float gy, float gz, float dt);

    /**
     * @brief Applies accelerometer-based correction to orientation.
     *
     * Uses accelerometer vector to estimate the direction of gravity and align the orientation accordingly.
     *
     * @param ax Acceleration X in m/s².
     * @param ay Acceleration Y in m/s².
     * @param az Acceleration Z in m/s².
     * @param dt Time delta since last update in seconds (not directly used, but can be useful for smoothing).
     */
    void apply_accel_influence(float ax, float ay, float az, float dt);

    /// Blending factor between gyroscope integration and accelerometer correction.
    float m_alpha;

    /// Timestamp of the last update in microseconds.
    int64_t m_last_timestamp;

    /// Current orientation estimate represented as a quaternion.
    glm::quat m_quat;
};

} // namespace kopter