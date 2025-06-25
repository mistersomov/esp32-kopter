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
 * @brief Interface for a motor mixer in a multirotor flight controller.
 *
 * This interface defines how control inputs such as base_throttle, roll, pitch, and yaw
 * are translated into individual motor outputs. Implementations of this interface
 * can define various mixing strategies depending on the frame layout (e.g., X, +, H),
 * motor orientation, and direction of rotation.
 *
 * The mixer acts as a bridge between the flight controller and motor drivers,
 * ensuring that control signals are appropriately distributed among throttles.
 */
struct IMotorMixer {
    virtual ~IMotorMixer() = default;

    /**
     * @brief Apply control signals to compute per-motor throttle outputs.
     *
     * @param throttles Array to write motor outputs to. Must be large enough for the given frame.
     * @param base_throttle Base throttle (0.0 to 1.0).
     * @param roll Roll command.
     * @param pitch Pitch command.
     * @param yaw Yaw command.
     */
    virtual void mix(float *throttles, float base_throttle, float roll, float pitch, float yaw) const = 0;
};

} // namespace kopter