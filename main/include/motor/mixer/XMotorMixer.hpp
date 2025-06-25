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

#include "IMotorMixer.hpp"

namespace kopter {

/**
 * @brief Mixer implementation for a quadcopter in "X" frame configuration.
 *
 * This class implements the motor mixing logic for quadcopters arranged in an "X" frame layout.
 * It translates control signals—collective throttle, roll, pitch, and yaw—into individual motor outputs.
 *
 * The standard "X" configuration consists of four motors:
 *
 *   - Motor 0: Front Left  (CCW)
 *   - Motor 1: Front Right (CW)
 *   - Motor 2: Rear Right  (CCW)
 *   - Motor 3: Rear Left   (CW)
 *
 * Each control input affects the motor outputs as follows:
 *   - `Throttle`: equally increases all motors.
 *   - `Roll`: adjusts left/right pairs (motors on the left decrease while right increase, and vice versa).
 *   - `Pitch`: adjusts front/back pairs (front motors decrease while rear increase, and vice versa).
 *   - `Yaw`: alternates CW/CCW motors to induce rotation.
 *
 * This mixer assumes motors are indexed in the following order:
 *   ```
 *        Front
 *         ^
 *         |
 *     [0]     [1]
 *        \   /
 *         \ /
 *         / \
 *     [3]     [2]
 *         |
 *        Back
 *   ```
 *
 * @note Output values are written directly into the `throttles` array, which should contain at least 4 elements.
 * @note It is the caller's responsibility to clamp values to [0.0f, 1.0f] and apply actual PWM scaling.
 */
struct XMotorMixer : public IMotorMixer {

    /**
     * @brief Mix control inputs into motor outputs.
     *
     * @param throttles Pointer to an array of at least 4 floats where the resulting motor outputs will be stored.
     * @param base_throttle Collective throttle value, typically in [0.0, 1.0].
     * @param roll Roll input (left/right tilt), typically in [-1.0, 1.0].
     * @param pitch Pitch input (forward/backward tilt), typically in [-1.0, 1.0].
     * @param yaw Yaw input (rotation), typically in [-1.0, 1.0].
     */
    void mix(float *throttles, float base_throttle, float roll, float pitch, float yaw) const override;
};

} // namespace kopter