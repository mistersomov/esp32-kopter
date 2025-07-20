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

#include "IMotor.hpp"

#include "driver/ledc.h"
#include "soc/gpio_num.h"

namespace kopter {

/**
 * @brief Represents a brushed DC (BDC) motor controlled via MCPWM on ESP-IDF.
 *
 * This class provides a C++ interface to control a brushed DC motor using the ESP-IDF
 * `bdc_motor` driver. It implements the `IMotor` interface, allowing clients to
 * enable or disable the motor, set its speed, and control its direction.
 *
 * Internally, it uses the MCPWM peripheral configured at a default frequency of 25 kHz
 * with a timer resolution of 80 MHz.
 */
class BDCMotor : public IMotor {
public:
    /**
     * @brief Constructs a BDCMotor instance.
     *
     * Initializes the motor with the specified PWM GPIO pin and LEDC channel configuration.
     *
     * @param gpio The GPIO number connected to the motor's PWM control input.
     * @param channel_cfg The pre-configured LEDC channel configuration.
     * @param duty_max Maximum duty cycle value according to timer resolution.
     *
     * @throws MotorException if initialization of the motor driver fails.
     */
    explicit BDCMotor(gpio_num_t gpio, const ledc_channel_config_t &channel_cfg, const uint32_t duty_max);

    /**
     * @brief Dtor for BDCMotor.
     *
     * Releases resources associated with the motor driver.
     */
    ~BDCMotor() override = default;

    /**
     * @brief Enables the motor hardware and prepares it for operation.
     *
     * @throws MotorException if the enable operation fails.
     */
    void enable() override;

    /**
     * @brief Disables the motor, stopping its operation and putting it in an inactive state.
     *
     * @throws MotorException if disabling the motor fails.
     */
    void disable() override;

    /**
     * @brief Sets the speed of the motor using a PWM duty cycle.
     *
     * The speed parameter controls the PWM duty cycle applied to the motor.
     *
     * @param speed Desired motor speed.
     *
     * @throws MotorException if setting the speed fails.
     */
    void set_speed(float speed) override;

private:
    ledc_channel_config_t m_channel_cfg;
    uint32_t m_duty_max;
};

} // namespace kopter