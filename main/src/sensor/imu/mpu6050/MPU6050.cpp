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

namespace kopter {

namespace {
constexpr uint8_t POWER_MGT_REG = 0x6B;
constexpr uint8_t DEVICE_RESET = 0x01;
constexpr uint8_t SLEEP_MODE = 0x00;
constexpr uint8_t ACCEL_CONFIG_REG = 0x1C;
constexpr uint8_t GYRO_CONFIG_REG = 0x1B;
constexpr uint8_t ACCEL_2G = 0x00;
constexpr uint8_t GYRO_250DPS = 0x00;
constexpr uint8_t REG_AX_H = 0x3B;
constexpr uint8_t REG_AY_H = 0x3D;
constexpr uint8_t REG_AZ_H = 0x3F;
constexpr uint8_t REG_GX_H = 0x43;
constexpr uint8_t REG_GY_H = 0x45;
constexpr uint8_t REG_GZ_H = 0x47;
constexpr uint8_t BYTES_PER_AXIS = 2;
constexpr uint8_t DELAY_MS = 100;
} // namespace

MPU6050::MPU6050(uint8_t address) : IMU(), m_mapper{std::make_unique<MPU6050Mapper>()}
{
    m_i2c_device.reset(I2cDeviceHolder::get_instance().add_device("MPU6050", address));
    assert(m_i2c_device);
    set_config();
}

MPU6050::~MPU6050() = default;

const char *MPU6050::get_name() const noexcept
{
    return "[MPU6050]";
}

float MPU6050::read_ax() const
{
    auto raw = get_raw_value(REG_AX_H);
    return m_mapper->map_accel_x(raw);
}

float MPU6050::read_ay() const
{
    auto raw = get_raw_value(REG_AY_H);
    return m_mapper->map_accel_y(raw);
}

float MPU6050::read_az() const
{
    auto raw = get_raw_value(REG_AZ_H);
    return m_mapper->map_accel_z(raw);
}

float MPU6050::read_gx() const
{
    auto raw = get_raw_value(REG_GX_H);
    return m_mapper->map_gyro_x(raw);
}

float MPU6050::read_gy() const
{
    auto raw = get_raw_value(REG_GY_H);
    return m_mapper->map_gyro_y(raw);
}

float MPU6050::read_gz() const
{
    auto raw = get_raw_value(REG_GZ_H);
    return m_mapper->map_gyro_z(raw);
}

int16_t MPU6050::get_raw_value(uint8_t reg) const
{
    auto result = m_i2c_device->read(reg, BYTES_PER_AXIS);
    int16_t raw = static_cast<int16_t>((result[0] << 8) | result[1]);

    return raw;
}

void MPU6050::set_config()
{
    m_i2c_device->write({POWER_MGT_REG, DEVICE_RESET});
    vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
    m_i2c_device->write({POWER_MGT_REG, SLEEP_MODE});
    vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
    m_i2c_device->write({ACCEL_CONFIG_REG, ACCEL_2G});
    m_i2c_device->write({GYRO_CONFIG_REG, GYRO_250DPS});
}

} // namespace kopter