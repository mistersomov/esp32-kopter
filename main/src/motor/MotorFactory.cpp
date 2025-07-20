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
#include "MotorFactory.hpp"

#include "MotorException.hpp"

namespace kopter {

namespace {
constexpr ledc_mode_t SPEED_MODE = LEDC_LOW_SPEED_MODE;
constexpr ledc_timer_t TIMER = LEDC_TIMER_0;
constexpr ledc_timer_bit_t TIMER_RESOLUTION = LEDC_TIMER_12_BIT;
constexpr soc_periph_ledc_clk_src_legacy_t TIMER_CLK_CFG = LEDC_AUTO_CLK;
constexpr ledc_intr_type_t INTERRUPT_TYPE = LEDC_INTR_DISABLE;
constexpr uint32_t TIMER_FREQUENCY_HZ = 5000;
constexpr std::string_view TAG = "[MotorFactory]";
} // namespace

MotorFactory::MotorFactory()
{
    configure_timer();
    m_duty_max = 1U << static_cast<uint32_t>(m_timer_cfg.duty_resolution);
}

MotorFactory::~MotorFactory()
{
    try {
        m_timer_cfg.deconfigure = true;
        check_call<MotorException>(ledc_timer_config(&m_timer_cfg));
    }
    catch (const MotorException &e) {
        ESP_LOGE(TAG.data(), "Failed to destruct ledc timer.");
    }
}

MotorFactory &MotorFactory::get_instance()
{
    static MotorFactory instance;
    return instance;
}

std::unique_ptr<BDCMotor> MotorFactory::make_bdc_motor(const gpio_num_t gpio, const ledc_channel_t channel)
{
    auto channel_cfg = create_channel_cfg(gpio, channel);
    return std::make_unique<BDCMotor>(gpio, channel_cfg, m_duty_max - 1);
}

void MotorFactory::configure_timer()
{
    m_timer_cfg.speed_mode = SPEED_MODE;
    m_timer_cfg.timer_num = TIMER;
    m_timer_cfg.duty_resolution = TIMER_RESOLUTION;
    m_timer_cfg.clk_cfg = TIMER_CLK_CFG;
    m_timer_cfg.freq_hz = TIMER_FREQUENCY_HZ;

    check_call<MotorException>(ledc_timer_config(&m_timer_cfg));
}

const ledc_channel_config_t MotorFactory::create_channel_cfg(const gpio_num_t gpio, const ledc_channel_t channel) const
{
    ledc_channel_config_t channel_cfg{};
    channel_cfg.gpio_num = gpio;
    channel_cfg.channel = channel;
    channel_cfg.timer_sel = TIMER;
    channel_cfg.speed_mode = SPEED_MODE;
    channel_cfg.intr_type = INTERRUPT_TYPE;
    channel_cfg.duty = 0;
    channel_cfg.hpoint = 0;

    check_call<MotorException>(ledc_channel_config(&channel_cfg));

    return channel_cfg;
}

} // namespace kopter