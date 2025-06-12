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

#ifndef MOTOR_HPP
#define MOTOR_HPP

#include "bdc_motor.h"
#include "pid_ctrl.h"

namespace kopter {

class Motor {
public:
    explicit Motor(const uint8_t &gpio);
    ~Motor();

    void enable();
    void disable();
    void forward();
    void set_speed(uint16_t speed);

private:
    void init_motor(const uint8_t &gpio);
    void init_pid();

    pid_ctrl_block_handle_t m_pid{nullptr};
    bdc_motor_handle_t m_motor{nullptr};
};

} // namespace kopter

#endif