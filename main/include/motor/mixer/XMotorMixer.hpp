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
 * @brief Mixer implementation for quadcopters in the "X" configuration.
 *
 * Converts collective throttle, roll, pitch, and yaw inputs into motor throttle values for 4 motors:
 *
 *   - Motor 0: Front Left  (CW)
 *   - Motor 1: Front Right (CCW)
 *   - Motor 2: Rear Right  (CW)
 *   - Motor 3: Rear Left   (CCW)
 *
 * Layout (top-down view):
 * ```
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
 * ```
 *
 * @note Throttle output values are clamped to [0.0f, 1.0f].
 * @note Caller must ensure the `throttles` array in `MotorMixerConfig` has at least 4 elements.
 */
struct XMotorMixer : public IMotorMixer {

    /**
     * @brief Computes motor outputs based on input control signals.
     *
     * @param cfg Configuration containing input signals and target throttle array.
     */
    void mix(const MotorMixerConfig &cfg) const override;
};

} // namespace kopter