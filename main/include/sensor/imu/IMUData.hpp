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
 * @brief Structure representing raw Inertial Measurement Unit (IMU) data.
 *
 * Contains angular velocity (gyroscope) and linear acceleration (accelerometer) values
 * along all three axes (X, Y, Z). This data is typically produced by devices like MPU6050 or ICM-20948.
 */
struct IMUData {
    /// Angular velocity around X axis.
    float gx;

    /// Angular velocity around Y axis.
    float gy;

    /// Angular velocity around Z axis.
    float gz;

    /// Acceleration along X axis.
    float ax;

    /// Acceleration along Y axis.
    float ay;

    /// Acceleration along Z axis.
    float az;
};
} // namespace kopter