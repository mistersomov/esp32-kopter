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

#include "IMotor.hpp"
#include "MotorException.hpp"

namespace kopter {

BDCMotor::BDCMotor(gpio_num_t gpio, const ledc_channel_config_t &channel_cfg, const uint32_t duty_max)
    : IMotor(), m_channel_cfg{std::move(channel_cfg)}, m_duty_max{duty_max}
{
    set_speed(0.0f);
}

void BDCMotor::enable()
{
    set_speed(0.0f);
}

void BDCMotor::disable()
{
    check_call<MotorException>(ledc_stop(m_channel_cfg.speed_mode, m_channel_cfg.channel, 0));
}

void BDCMotor::set_speed(float speed)
{
    const uint32_t duty = static_cast<uint32_t>(speed * m_duty_max);

    check_call<MotorException>(ledc_set_duty(m_channel_cfg.speed_mode, m_channel_cfg.channel, duty));
    check_call<MotorException>(ledc_update_duty(m_channel_cfg.speed_mode, m_channel_cfg.channel));
}

} // namespace kopter