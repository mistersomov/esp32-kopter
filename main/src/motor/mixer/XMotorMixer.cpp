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

#include "pch.hpp"
#include "XMotorMixer.hpp"

namespace kopter {

namespace {
constexpr float MIN_THROTTLE = 0.0f;
constexpr float MAX_THROTTLE = 1.0f;
} // namespace

void XMotorMixer::mix(const MotorMixerConfig &cfg) const
{
    cfg.throttles[0] = std::clamp(cfg.collective_throttle - cfg.roll + cfg.pitch + cfg.yaw, MIN_THROTTLE, MAX_THROTTLE);
    cfg.throttles[1] = std::clamp(cfg.collective_throttle + cfg.roll + cfg.pitch - cfg.yaw, MIN_THROTTLE, MAX_THROTTLE);
    cfg.throttles[2] = std::clamp(cfg.collective_throttle + cfg.roll - cfg.pitch + cfg.yaw, MIN_THROTTLE, MAX_THROTTLE);
    cfg.throttles[3] = std::clamp(cfg.collective_throttle - cfg.roll - cfg.pitch - cfg.yaw, MIN_THROTTLE, MAX_THROTTLE);
}

} // namespace kopter