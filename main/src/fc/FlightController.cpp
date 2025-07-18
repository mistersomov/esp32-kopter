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
#include "FlightController.hpp"

namespace kopter {

static constexpr float BASE_THROTTLE = 0.8f;
static constexpr std::string_view TAG = "[FC]";

FlightController::FlightController(std::unique_ptr<IMU> imu,
                                   std::unique_ptr<IBarometer> barometer,
                                   std::unique_ptr<IOrientationFilter> orientation_filter,
                                   std::unique_ptr<IMotorMixer> motor_mixer,
                                   std::unique_ptr<PID> pid_roll,
                                   std::unique_ptr<PID> pid_pitch,
                                   std::unique_ptr<PID> pid_yaw,
                                   std::unique_ptr<PID> pid_altitude)
    : m_imu{std::move(imu)},
      m_barometer{std::move(barometer)},
      m_orientation_filter{std::move(orientation_filter)},
      m_motor_mixer{std::move(motor_mixer)},
      m_pid_roll{pid_roll ? std::move(pid_roll) : std::make_unique<PID>()},
      m_pid_pitch{pid_pitch ? std::move(pid_pitch) : std::make_unique<PID>()},
      m_pid_yaw{pid_yaw ? std::move(pid_yaw) : std::make_unique<PID>()},
      m_pid_altitude{pid_altitude ? std::move(pid_altitude) : std::make_unique<PID>()}
{
}

void FlightController::update_speed(uint64_t micros)
{
    m_orientation_filter->update(m_imu->read_gx(),
                                 m_imu->read_gy(),
                                 m_imu->read_gz(),
                                 m_imu->read_ax(),
                                 m_imu->read_ay(),
                                 m_imu->read_az(),
                                 micros);

    glm::vec3 eulers = glm::eulerAngles(m_orientation_filter->get_quat());

    float roll = glm::degrees(eulers.x);
    float pitch = glm::degrees(eulers.y);
    float yaw = glm::degrees(eulers.z);
    float altitude = m_barometer->read_altitude();

    float target_altitude = 400.0f;

    float output_pitch, output_roll, output_yaw, output_altitude;
    m_pid_roll->update(roll, output_roll);
    m_pid_pitch->update(pitch, output_pitch);
    m_pid_yaw->update(yaw, output_yaw);
    m_pid_altitude->update(target_altitude - altitude, output_altitude);

    float throttles[4] = {};
    const MotorMixerConfig cfg{.throttles = throttles,
                               .collective_throttle = BASE_THROTTLE,
                               .roll = output_roll,
                               .pitch = output_pitch,
                               .yaw = output_yaw};
    m_motor_mixer->mix(cfg);

    m_motors[0]->set_speed(throttles[0]);
    m_motors[1]->set_speed(throttles[1]);
    m_motors[2]->set_speed(throttles[2]);
    m_motors[3]->set_speed(throttles[3]);
}

} // namespace kopter