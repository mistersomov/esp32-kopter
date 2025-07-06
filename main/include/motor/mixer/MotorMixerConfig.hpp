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
 * @brief Configuration for motor mixer input and output.
 *
 * Contains references to control inputs and an array where motor throttle outputs will be written.
 *
 * @note The `throttles` array must contain at least 4 elements for quadcopters.
 * @note All values are expected to be normalized to the range [-1.0f, 1.0f], except base_throttle.
 */
struct MotorMixerConfig {
    /**
     * @brief Pointer to an array where computed throttle values will be written.
     *
     * The array must have at least 4 elements. Values will be clamped to the [0.0f, 1.0f] range.
     */
    float *throttles;

    /**
     * @brief Collective throttle input applied to all motors equally.
     *
     * Typically ranges from 0.0 (off) to 1.0 (full thrust).
     */
    float collective_throttle;

    /**
     * @brief Roll input (left/right tilt control).
     *
     * Negative values tilt left, positive values tilt right.
     */
    float roll;

    /**
     * @brief Pitch input (forward/backward tilt control).
     *
     * Negative values pitch down (forward), positive values pitch up (backward).
     */
    float pitch;

    /**
     * @brief Yaw input (rotational control).
     *
     * Negative values rotate counterclockwise (CCW), positive values rotate clockwise (CW).
     */
    float yaw;
};

} // namespace kopter