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

#include "BDCMotor.hpp"

#include "soc/gpio_num.h"

namespace kopter {

/**
 * @brief Factory class for creating and configuring motors.
 *
 * This singleton class manages the configuration of the LEDC timer used for PWM control of motors,
 * and provides a method to create instances of motors configured with proper GPIO and PWM channel.
 */
class MotorFactory {
public:
    MotorFactory(const MotorFactory &) = delete;
    MotorFactory &operator=(const MotorFactory &) = delete;

    /**
     * @brief Dtor that attempts to deconfigure the LEDC timer.
     */
    ~MotorFactory();

    /**
     * @brief Returns the singleton instance of MotorFactory.
     *
     * @return MotorFactory& Reference to the single MotorFactory instance.
     */
    static MotorFactory &get_instance();

    /**
     * @brief Creates a BDCMotor instance configured on the given GPIO pin and PWM channel.
     *
     * @param gpio GPIO number to be used for PWM output.
     * @param channel LEDC PWM channel number.
     * @return std::unique_ptr<BDCMotor> Unique pointer to the created BDCMotor instance.
     *
     * @throws MotorException if LEDC channel configuration fails.
     */
    std::unique_ptr<BDCMotor> make_bdc_motor(const gpio_num_t gpio, const ledc_channel_t channel);

private:
    /**
     * @brief Ctor that configures the LEDC timer.
     */
    MotorFactory();

    /**
     * @brief Configures the LEDC timer with predefined settings.
     *
     * @throws MotorException if timer configuration fails.
     */
    void configure_timer();

    /**
     * @brief Creates a LEDC channel configuration structure.
     *
     * @param gpio GPIO number to be assigned to the channel.
     * @param channel LEDC channel number.
     * @return ledc_channel_config_t Configured LEDC channel configuration struct.
     *
     * @throws MotorException if channel configuration fails.
     */
    [[nodiscard]] const ledc_channel_config_t create_channel_cfg(const gpio_num_t gpio,
                                                                 const ledc_channel_t channel) const;

    ledc_timer_config_t m_timer_cfg;
    uint32_t m_duty_max;
};
} // namespace kopter