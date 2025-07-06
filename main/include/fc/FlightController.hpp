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

#include "IBarometer.hpp"
#include "IMotor.hpp"
#include "IMotorMixer.hpp"
#include "IMU.hpp"
#include "IOrientationFilter.hpp"
#include "PID.hpp"

namespace kopter {

/**
 * @brief High-level flight controller that manages stabilization and motor control.
 *
 * The `FlightController` class is responsible for reading sensor data, estimating orientation,
 * computing control signals via PID controllers, and setting motor speeds accordingly.
 *
 * It integrates multiple sensor interfaces (IMU, barometer), an orientation filter, a motor mixer,
 * and PID regulators for each of the four controlled axes: roll, pitch, yaw, and altitude.
 *
 * Example usage:
 * ```
 * auto controller = std::make_unique<FlightController>(
 *     std::make_unique<MyIMU>(),
 *     std::make_unique<MyBarometer>(),
 *     std::make_unique<MyOrientationFilter>(),
 *     std::make_unique<MyMotorMixer>(),
 *     std::make_unique<PID>(1.0f, 0.1f, 0.01f), // roll
 *     std::make_unique<PID>(1.0f, 0.1f, 0.01f), // pitch
 *     std::make_unique<PID>(0.8f, 0.05f, 0.02f), // yaw
 *     std::make_unique<PID>(1.5f, 0.2f, 0.05f)); // altitude
 * controller->update_speed();
 * ```
 */
class FlightController final {
public:
    /**
     * @brief Ctor for a new FlightController instance.
     *
     * @param imu Inertial Measurement Unit providing accelerometer and gyroscope data.
     * @param barometer Sensor used to measure atmospheric pressure and estimate altitude.
     * @param orientation_filter Filter that estimates orientation as a quaternion.
     * @param motor_mixer Mixer that translates control signals into motor throttle values.
     * @param pid_roll Optional custom PID controller for roll. If nullptr, a default controller is used.
     * @param pid_pitch Optional custom PID controller for pitch. If nullptr, a default controller is used.
     * @param pid_yaw Optional custom PID controller for yaw. If nullptr, a default controller is used.
     * @param pid_altitude Optional custom PID controller for altitude. If nullptr, a default controller is used.
     */
    FlightController(std::unique_ptr<IMU> imu,
                     std::unique_ptr<IBarometer> barometer,
                     std::unique_ptr<IOrientationFilter> orientation_filter,
                     std::unique_ptr<IMotorMixer> motor_mixer,
                     std::unique_ptr<PID> pid_roll = nullptr,
                     std::unique_ptr<PID> pid_pitch = nullptr,
                     std::unique_ptr<PID> pid_yaw = nullptr,
                     std::unique_ptr<PID> pid_altitude = nullptr);

    /**
     * @brief Reads sensors, computes control outputs, and updates motor speeds.
     *
     * This method performs one iteration of the flight control loop. It:
     * - Reads raw IMU data (gyroscope and accelerometer).
     * - Updates orientation via the quaternion filter.
     * - Converts orientation to Euler angles (roll, pitch, yaw).
     * - Reads barometric altitude.
     * - Computes PID outputs for all axes.
     * - Mixes outputs into individual motor throttle values.
     * - Updates motor speeds accordingly.
     *
     * This function should be called periodically, ideally at a fixed frequency (e.g., every 10–20 ms).
     */
    void update_speed(uint64_t micros);

private:
    std::unique_ptr<IMU> m_imu;
    std::unique_ptr<IBarometer> m_barometer;
    std::unique_ptr<IOrientationFilter> m_orientation_filter;
    std::unique_ptr<IMotorMixer> m_motor_mixer;
    std::array<std::unique_ptr<IMotor>, 4> m_motors;
    std::unique_ptr<PID> m_pid_roll;
    std::unique_ptr<PID> m_pid_pitch;
    std::unique_ptr<PID> m_pid_yaw;
    std::unique_ptr<PID> m_pid_altitude;
};

} // namespace kopter