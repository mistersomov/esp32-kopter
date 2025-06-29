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
#include "ComplementaryFilter.hpp"

#include <glm/gtc/constants.hpp>

namespace kopter {

static constexpr float MS2SEC = 1e6f;
static constexpr float DEG2RAD = glm::pi<float>() / 180.0f;
static constexpr float MIN_ANGLE = 1e-6f;

ComplementaryFilter::ComplementaryFilter(float alpha) noexcept
    : m_alpha{alpha}, m_last_timestamp{0}, m_quat{glm::vec3(0.0f)}
{
}

const glm::quat &ComplementaryFilter::get_quat() const noexcept
{
    return m_quat;
}

void ComplementaryFilter::update(float gx, float gy, float gz, float ax, float ay, float az, int64_t timestamp_us)
{
    if (m_last_timestamp == 0) {
        m_last_timestamp = timestamp_us;
        return;
    }

    float dt = (timestamp_us - m_last_timestamp) / MS2SEC;
    m_last_timestamp = timestamp_us;

    apply_gyro_influence(gx, gy, gz, dt);
    apply_accel_influence(ax, ay, az, dt);
}

void ComplementaryFilter::apply_gyro_influence(float gx, float gy, float gz, float dt)
{
    gx *= DEG2RAD;
    gy *= DEG2RAD;
    gz *= DEG2RAD;
    glm::vec3 omega(gx, gy, gz);
    float angle = glm::length(omega) * dt;
    glm::quat gyro_delta =
        (angle > MIN_ANGLE) ? glm::angleAxis(angle, glm::normalize(omega)) : glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    m_quat = glm::normalize(gyro_delta * m_quat);
}

void ComplementaryFilter::apply_accel_influence(float ax, float ay, float az, float dt)
{
    glm::vec3 accel = glm::normalize(glm::vec3(ax, ay, az));
    float pitch = std::atan2f(-accel.x, sqrtf(accel.y * accel.y + accel.z * accel.z));
    float roll = std::atan2f(accel.y, accel.z);
    glm::quat accel_quat = glm::angleAxis(roll, glm::vec3(1, 0, 0)) * glm::angleAxis(pitch, glm::vec3(0, 1, 0));
    m_quat = glm::normalize(glm::slerp(m_quat, accel_quat, 1.0f - m_alpha));
}

} // namespace kopter