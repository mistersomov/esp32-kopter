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
#include "MPU6050Mapper.hpp"

namespace kopter {

static constexpr float AFS_SEL_0 = 16384.0f;
static constexpr float AFS_SEL_1 = 8192.0f;
static constexpr float AFS_SEL_2 = 4096.0f;
static constexpr float AFS_SEL_3 = 2048.0f;
static constexpr float GFS_SEL_0 = 131.0f;
static constexpr float GFS_SEL_1 = 65.5f;
static constexpr float GFS_SEL_2 = 32.8f;
static constexpr float GFS_SEL_3 = 16.4f;

MPU6050Mapper::MPU6050Mapper(AccelSensitivityMode ax_mode,
                             AccelSensitivityMode ay_mode,
                             AccelSensitivityMode az_mode,
                             GyroSensitivityMode gx_mode,
                             GyroSensitivityMode gy_mode,
                             GyroSensitivityMode gz_mode) noexcept
    : m_ax_mode{ax_mode},
      m_ay_mode{ay_mode},
      m_az_mode{az_mode},
      m_gx_mode{gx_mode},
      m_gy_mode{gy_mode},
      m_gz_mode{gz_mode}
{
}

float MPU6050Mapper::map_accel_x(int16_t raw) const
{
    return map_accel(raw, m_ax_mode);
}

float MPU6050Mapper::map_accel_y(int16_t raw) const
{
    return map_accel(raw, m_ay_mode);
}

float MPU6050Mapper::map_accel_z(int16_t raw) const
{
    return map_accel(raw, m_az_mode);
}

float MPU6050Mapper::map_gyro_x(int16_t raw) const
{
    return map_gyro(raw, m_gx_mode);
}

float MPU6050Mapper::map_gyro_y(int16_t raw) const
{
    return map_gyro(raw, m_gy_mode);
}

float MPU6050Mapper::map_gyro_z(int16_t raw) const
{
    return map_gyro(raw, m_gz_mode);
}

float MPU6050Mapper::map_accel(int16_t value, AccelSensitivityMode mode) const
{
    switch (mode) {
    case AccelSensitivityMode::A2G:
        return value / AFS_SEL_0;
    case AccelSensitivityMode::A4G:
        return value / AFS_SEL_1;
    case AccelSensitivityMode::A8G:
        return value / AFS_SEL_2;
    case AccelSensitivityMode::A16G:
        return value / AFS_SEL_3;
    default:
        return value / AFS_SEL_0;
    }
}

float MPU6050Mapper::map_gyro(int16_t value, GyroSensitivityMode mode) const
{
    switch (mode) {
    case GyroSensitivityMode::DPS_250:
        return value / GFS_SEL_0;
    case GyroSensitivityMode::DPS_500:
        return value / GFS_SEL_1;
    case GyroSensitivityMode::DPS_1000:
        return value / GFS_SEL_2;
    case GyroSensitivityMode::DPS_2000:
        return value / GFS_SEL_3;
    default:
        return value / GFS_SEL_0;
    }
};

} // namespace kopter