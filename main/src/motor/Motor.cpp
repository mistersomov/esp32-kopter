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
#include "motor/Motor.hpp"

namespace kopter {

namespace {

// Customize region begin
constexpr uint16_t MCPWM_FREQ_HZ = 25000;                // 25KHz PWM
constexpr uint32_t MCPWM_TIMER_RESOLUTION_HZ = 10000000; // 10MHz, 1 tick = 0.1us
constexpr uint16_t MCPWM_DUTY_TICK_MAX =
    MCPWM_TIMER_RESOLUTION_HZ / MCPWM_FREQ_HZ; // maximum value we can set for the duty cycle, in ticks
constexpr float K_P = 0.6;
constexpr float K_I = 0.4;
constexpr float K_D = 0.2;
constexpr float PID_MAX_OUTPUT = MCPWM_DUTY_TICK_MAX - 1;
constexpr float PID_MIN_OUTPUT = 0;
constexpr float PID_INTEGRAL_MAX = 1000;
constexpr float PID_INTEGRAL_MIN = -1000;
// Customize region end
constexpr std::string_view TAG = "[Motor]";

} // namespace

Motor::Motor(const uint8_t &gpio)
{
    init_motor(gpio);
    init_pid();
}

Motor::~Motor()
{
    if (m_motor) {
        CHECK_THROW(bdc_motor_disable(m_motor));
        CHECK_THROW(bdc_motor_del(m_motor));
    }
    if (m_pid) {
        CHECK_THROW(pid_del_control_block(m_pid));
    }
}

void Motor::enable()
{
    CHECK_THROW(bdc_motor_enable(m_motor));
}

void Motor::disable()
{
    CHECK_THROW(bdc_motor_disable(m_motor));
}

void Motor::forward()
{
    CHECK_THROW(bdc_motor_forward(m_motor));
}

void Motor::set_speed(uint16_t speed)
{
    float scaled = (3300 - speed) * MCPWM_DUTY_TICK_MAX / 3300;
    float new_speed = 0.0f;

    // pid_compute(m_pid, scaled, &new_speed);
    try {
        CHECK_THROW(bdc_motor_set_speed(m_motor, static_cast<uint32_t>(scaled)));
    }
    catch (const idf::ESPException &e) {
        ESP_LOGE(TAG.data(), "ERROR: %s (0x%X)", e.what(), e.error);
    }
}

void Motor::init_motor(const uint8_t &gpio)
{
    bdc_motor_config_t motor_config{};
    motor_config.pwm_freq_hz = MCPWM_FREQ_HZ;
    motor_config.pwma_gpio_num = gpio;

    bdc_motor_mcpwm_config_t mcpwm_config{};
    mcpwm_config.group_id = 0;
    mcpwm_config.resolution_hz = MCPWM_TIMER_RESOLUTION_HZ;

    CHECK_THROW(bdc_motor_new_mcpwm_device(&motor_config, &mcpwm_config, &m_motor));
}
void Motor::init_pid()
{
    pid_ctrl_parameter_t pid_runtime_param{};
    pid_runtime_param.kp = K_P;
    pid_runtime_param.ki = K_I;
    pid_runtime_param.kd = K_D;
    pid_runtime_param.cal_type = PID_CAL_TYPE_INCREMENTAL;
    pid_runtime_param.max_output = PID_MAX_OUTPUT;
    pid_runtime_param.min_output = PID_MIN_OUTPUT;
    pid_runtime_param.max_integral = PID_INTEGRAL_MAX;
    pid_runtime_param.min_integral = PID_INTEGRAL_MIN;

    pid_ctrl_config_t pid_config{};
    pid_config.init_param = pid_runtime_param;

    CHECK_THROW(pid_new_control_block(&pid_config, &m_pid));
}
} // namespace kopter