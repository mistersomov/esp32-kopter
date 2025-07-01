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

#include "pid_ctrl.h"

namespace kopter {

/**
 * @class PID
 * @brief A wrapper class for the ESP-IDF PID controller module.
 *
 * This class encapsulates the configuration and usage of a PID control block
 * provided by the ESP-IDF `pid_ctrl` component. It simplifies the interface for
 * updating the PID controller with target and current values.
 *
 * Typical usage:
 * ```
 * PID pid;
 * pid.update(target_value, measured_value);
 * ```
 */
class PID {
public:
    /**
     * @brief Constructs and initializes the PID control block with default parameters.
     *
     * @param kp Proportional gain.
     * @param ki Integral gain.
     * @param kd Derivative gain.
     *
     * This constructor creates a new instance of the ESP-IDF PID controller using
     * default tuning parameters (Kp, Ki, Kd) and sets the output limits from `0.0f` to `1.0f`.
     *
     * The controller is ready to use immediately after construction. If needed,
     * further tuning can be done by modifying the PID parameters in the implementation.
     *
     * @throws PIDException if the PID control block could not be created or initialized.
     */
    PID(float kp = 1.0f, float ki = 0.1f, float kd = 0.05f);

    /**
     * Dtor with deleting a `pid_ctrl_block_handle_t` member.
     *
     * @throws PIDException if `pid_ctrl_block_handle_t` destruction has failed.
     */
    ~PID();

    /**
     * @brief Updates the PID controller with the latest target and measured values.
     *
     * @param current_error Difference between the setpoint and the current value.
     * @param output Calculated the resulting output value.
     *
     * Internally, this method calculates the error, updates the PID, and applies
     * the resulting control output.
     *
     * @throws PIDException if PID calculation has failed.
     */
    void update(float current_error, float &output);

private:
    pid_ctrl_block_handle_t m_pid;
};

} // namespace kopter