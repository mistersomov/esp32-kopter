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

namespace {
constexpr float MAX_OUTPUT = 100.0f;
constexpr float MIN_OUTPUT = -100.0f;
constexpr float MAX_INTEGRAL = 1000.0f;
constexpr float MIN_INTEGRAL = -1000.0f;
} // namespace

PID::PID(float kp, float ki, float kd, float target_point) : m_target_point{target_point}
{
    m_pid_runtime_param.kp = kp;
    m_pid_runtime_param.ki = ki;
    m_pid_runtime_param.kd = kd;
    m_pid_runtime_param.cal_type = PID_CAL_TYPE_POSITIONAL;
    m_pid_runtime_param.max_output = MAX_OUTPUT;
    m_pid_runtime_param.min_output = MIN_OUTPUT;
    m_pid_runtime_param.max_integral = MAX_INTEGRAL;
    m_pid_runtime_param.min_integral = MIN_INTEGRAL;

    pid_ctrl_config_t pid_config{};
    pid_config.init_param = m_pid_runtime_param;

    check_call<PIDException>(pid_new_control_block(&pid_config, &m_pid));
}

PID::~PID()
{
    if (m_pid) {
        check_call<PIDException>(pid_del_control_block(m_pid));
    }
}

void PID::update(float current, float &output)
{
    check_call<PIDException>(pid_compute(m_pid, m_target_point - current, &output));
}

void PID::set_target_point(float value) noexcept
{
    m_target_point = value;
}

void PID::set_kp(float value)
{
    m_pid_runtime_param.kp = value;
    check_call<PIDException>(pid_update_parameters(m_pid, &m_pid_runtime_param));
}

void PID::set_ki(float value)
{
    m_pid_runtime_param.ki = value;
    check_call<PIDException>(pid_update_parameters(m_pid, &m_pid_runtime_param));
}

void PID::set_kd(float value)
{
    m_pid_runtime_param.kd = value;
    check_call<PIDException>(pid_update_parameters(m_pid, &m_pid_runtime_param));
}

} // namespace kopter