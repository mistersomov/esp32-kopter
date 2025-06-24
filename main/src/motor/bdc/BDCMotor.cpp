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
#include "BDCMotor.hpp"

#include "MotorException.hpp"

namespace kopter {

constexpr uint8_t MCPWM_GROUP_ID = 0;
constexpr uint32_t MCPWM_FREQ_HZ = 24500;                // 25KHz PWM
constexpr uint32_t MCPWM_TIMER_RESOLUTION_HZ = 80000000; // 80MHz, 1 tick = 0.1us
constexpr uint32_t MCPWM_DUTY_TICK_MAX =
    MCPWM_TIMER_RESOLUTION_HZ / MCPWM_FREQ_HZ; // maximum value we can set for the duty cycle, in ticks

BDCMotor::BDCMotor(const std::string &name, gpio_num_t gpio) : IMotor(std::move(name))
{
    bdc_motor_config_t motor_config{};
    motor_config.pwm_freq_hz = MCPWM_FREQ_HZ;
    motor_config.pwma_gpio_num = gpio;

    bdc_motor_mcpwm_config_t mcpwm_config{};
    mcpwm_config.group_id = MCPWM_GROUP_ID;
    mcpwm_config.resolution_hz = MCPWM_TIMER_RESOLUTION_HZ;

    check_call<MotorException>(
        [&]() { CHECK_THROW(bdc_motor_new_mcpwm_device(&motor_config, &mcpwm_config, &m_motor)); });
    enable();
}

BDCMotor::~BDCMotor()
{
    if (m_motor) {
        check_call<MotorException>([this]() { CHECK_THROW(bdc_motor_del(m_motor)); });
    }
}

void BDCMotor::enable()
{
    check_call<MotorException>([this]() { CHECK_THROW(bdc_motor_enable(m_motor)); });
}

void BDCMotor::disable()
{
    check_call<MotorException>([this]() { CHECK_THROW(bdc_motor_disable(m_motor)); });
}

void BDCMotor::set_speed(float speed)
{
    check_call<MotorException>([speed, this]() {
        CHECK_THROW(bdc_motor_set_speed(m_motor, static_cast<uint32_t>(speed * MCPWM_DUTY_TICK_MAX)));
    });
}

void BDCMotor::coast()
{
    check_call<MotorException>([this]() { CHECK_THROW(bdc_motor_coast(m_motor)); });
}

void BDCMotor::forward()
{
    check_call<MotorException>([this]() { CHECK_THROW(bdc_motor_forward(m_motor)); });
}

void BDCMotor::reverse()
{
    check_call<MotorException>([this]() { CHECK_THROW(bdc_motor_reverse(m_motor)); });
}

} // namespace kopter