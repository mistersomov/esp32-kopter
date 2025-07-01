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
#include "PID.hpp"

#include "PIDException.hpp"

namespace kopter {

static constexpr float MAX_OUTPUT = 1.0f;
static constexpr float MIN_OUTPUT = 0.0f;
static constexpr float MAX_INTEGRAL = 1000.0f;
static constexpr float MIN_INTEGRAL = -1000.0f;

PID::PID(float kp, float ki, float kd)
{
    pid_ctrl_parameter_t pid_runtime_param{};
    pid_runtime_param.kp = kp;
    pid_runtime_param.ki = ki;
    pid_runtime_param.kd = kd;
    pid_runtime_param.cal_type = PID_CAL_TYPE_INCREMENTAL;
    pid_runtime_param.max_output = MAX_OUTPUT;
    pid_runtime_param.min_output = MIN_OUTPUT;
    pid_runtime_param.max_integral = MAX_INTEGRAL;
    pid_runtime_param.min_integral = MIN_INTEGRAL;

    pid_ctrl_config_t pid_config{};
    pid_config.init_param = pid_runtime_param;

    check_call<PIDException>([&]() { pid_new_control_block(&pid_config, &m_pid); });
}

PID::~PID()
{
    if (m_pid) {
        check_call<PIDException>([this]() { pid_del_control_block(m_pid); });
    }
}

void PID::update(float current_error, float &output)
{
    check_call<PIDException>([&]() { pid_compute(m_pid, current_error, &output); });
}

} // namespace kopter