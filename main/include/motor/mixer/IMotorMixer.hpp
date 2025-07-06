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

#include "MotorMixerConfig.hpp"

namespace kopter {

/**
 * @brief Abstract interface for a motor mixer in a multirotor flight controller.
 *
 * A motor mixer maps control signals (throttle, roll, pitch, yaw) into per-motor outputs.
 * The output is written into a throttle array passed in via `MotorMixerConfig`.
 */
struct IMotorMixer {
    virtual ~IMotorMixer() = default;

    /**
     * @brief Apply control signals to compute per-motor throttle outputs.
     *
     * @param cfg Structure containing input signals and a target buffer for motor outputs.
     */
    virtual void mix(const MotorMixerConfig &cfg) const = 0;
};

} // namespace kopter