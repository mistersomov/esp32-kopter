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
 * @brief Flight modes of the drone.
 *
 * Defines different operational modes controlling stabilization and autonomy:
 *
 * - STABILIZE: Basic attitude stabilization with manual control.
 * - ALTITUDE_HOLD: Maintains altitude using barometer while allowing manual pitch/roll/yaw control.
 * - POSITION_HOLD: Maintains position using GPS and other sensors.
 * - FAILSAFE: Safety mode activated on signal loss or critical errors.
 */
enum class FlightMode : char {
    STABILIZE,
    ALTITUDE_HOLD,
    POSITION_HOLD,
    FAILSAFE
};
} // namespace kopter