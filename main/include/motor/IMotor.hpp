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

#include "Device.hpp"

namespace kopter {

/**
 * @interface IMotor
 * @brief Interface representing a motor device.
 *
 * This structure defines a pure virtual interface for a motor, providing
 * methods for enabling, disabling, and controlling the direction and speed.
 * All implementations of this interface must provide definitions for the
 * listed methods.
 *
 * Inherits from the Device class.
 */
struct IMotor : public Device {
    /**
     * @brief Ctor with the given name.
     *
     * @param name The name of the motor device.
     */
    IMotor(const std::string &name);

    /**
     * @brief Virtual dtor for Motor.
     */
    virtual ~IMotor() = default;

    /**
     * @brief Enable the motor.
     *
     * Typically powers up or activates the motor driver to allow motion.
     */
    virtual void enable() = 0;

    /**
     * @brief Disable the motor.
     *
     * Typically powers down or deactivates the motor driver to stop all motion.
     */
    virtual void disable() = 0;

    /**
     * @brief Set the speed of the motor.
     *
     * @param speed The speed value to set, typically in the range 0–MAX.
     *              Exact meaning depends on motor implementation.
     */
    virtual void set_speed(float speed) = 0;
};

} // namespace kopter