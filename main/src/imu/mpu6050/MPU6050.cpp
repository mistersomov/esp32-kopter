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
#include "MPU6050.hpp"

#include "I2cDeviceHolder.hpp"
#include "IMU.hpp"

namespace kopter {

static constexpr uint8_t AX_UPPER_BYTE = 0x3B;
static constexpr uint8_t AY_UPPER_BYTE = 0x3D;
static constexpr uint8_t AZ_UPPER_BYTE = 0x3F;
static constexpr uint8_t GX_UPPER_BYTE = 0x43;
static constexpr uint8_t GY_UPPER_BYTE = 0x45;
static constexpr uint8_t GZ_UPPER_BYTE = 0x47;
static constexpr uint8_t BYTES_PER_AXIS = 2;

MPU6050::MPU6050(const uint8_t address) : IMU(), m_mapper{std::make_unique<MPU6050Mapper>()}
{
    m_i2c_device.reset(
        static_cast<I2cDevice *>(I2cDeviceHolder::get_instance().add_device(get_name(), std::move(address))));

    assert(m_i2c_device);

    m_i2c_device->write({0x6B, 0x00});
    vTaskDelay(pdMS_TO_TICKS(100));
}

const char *MPU6050::get_name() const noexcept
{
    return "[MPU6050]";
}

float MPU6050::read_ax() const
{
    auto raw = get_raw_value(AX_UPPER_BYTE);
    float ax = m_mapper->map_accel_x(raw);

    return ax;
}

float MPU6050::read_ay() const
{
    auto raw = get_raw_value(AY_UPPER_BYTE);
    float ay = m_mapper->map_accel_y(raw);

    return ay;
}

float MPU6050::read_az() const
{
    auto raw = get_raw_value(AZ_UPPER_BYTE);
    float az = m_mapper->map_accel_z(raw);

    return az;
}

float MPU6050::read_gx() const
{
    auto raw = get_raw_value(GX_UPPER_BYTE);
    float gx = m_mapper->map_gyro_x(raw);

    return gx;
}

float MPU6050::read_gy() const
{
    auto raw = get_raw_value(GY_UPPER_BYTE);
    float gy = m_mapper->map_gyro_y(raw);

    return gy;
}

float MPU6050::read_gz() const
{
    auto raw = get_raw_value(GZ_UPPER_BYTE);
    float gz = m_mapper->map_gyro_z(raw);

    return gz;
}

int16_t MPU6050::get_raw_value(uint8_t reg) const
{
    auto result = m_i2c_device->read(reg, BYTES_PER_AXIS);
    int16_t raw = (result[0] << 8) | result[1];

    return raw;
}

} // namespace kopter