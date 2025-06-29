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
#include "MPU6050Mapper.hpp"

namespace kopter {

static constexpr uint8_t POWER_MGT_REG = 0x6B;
static constexpr uint8_t DEVICE_RESET = 0x01;
static constexpr uint8_t SLEEP_MODE = 0x00;
static constexpr uint8_t ACCEL_CONFIG_REG = 0x1C;
static constexpr uint8_t GYRO_CONFIG_REG = 0x1B;
static constexpr uint8_t ACCEL_2G = 0x00;
static constexpr uint8_t GYRO_250DPS = 0x00;
static constexpr uint8_t REG_AX_H = 0x3B;
static constexpr uint8_t REG_AY_H = 0x3D;
static constexpr uint8_t REG_AZ_H = 0x3F;
static constexpr uint8_t REG_GX_H = 0x43;
static constexpr uint8_t REG_GY_H = 0x45;
static constexpr uint8_t REG_GZ_H = 0x47;
static constexpr uint8_t BYTES_PER_AXIS = 2;
static constexpr uint8_t DELAY_MS = 100;

class MPU6050::MPU6050Impl {
public:
    explicit MPU6050Impl(uint8_t address) : m_mapper{std::make_unique<MPU6050Mapper>()}
    {
        m_i2c_device.reset(I2cDeviceHolder::get_instance().add_device("MPU6050", address));
        assert(m_i2c_device);
        set_config();
    }

    float read_ax() const
    {
        auto raw = get_raw_value(REG_AX_H);
        return m_mapper->map_accel_x(raw);
    }

    float read_ay() const
    {
        auto raw = get_raw_value(REG_AY_H);
        return m_mapper->map_accel_y(raw);
    }

    float read_az() const
    {
        auto raw = get_raw_value(REG_AZ_H);
        return m_mapper->map_accel_z(raw);
    }

    float read_gx() const
    {
        auto raw = get_raw_value(REG_GX_H);
        return m_mapper->map_gyro_x(raw);
    }

    float read_gy() const
    {
        auto raw = get_raw_value(REG_GY_H);
        return m_mapper->map_gyro_y(raw);
    }

    float read_gz() const
    {
        auto raw = get_raw_value(REG_GZ_H);
        return m_mapper->map_gyro_z(raw);
    }

private:
    /**
     * @brief Reads raw 16-bit sensor data from the specified register.
     *
     * This function reads two consecutive bytes starting from the given register address
     * over I2C and combines them into a signed 16-bit integer. This raw value is typically
     * used for internal conversion into physical units (e.g., acceleration or rotation rate).
     *
     * @param reg The starting register address to read from.
     * @return `int16_t` The raw 16-bit signed value from the sensor.
     *
     * @throws I2cException with the corrsponding esp_err_t return value if something goes wrong
     */
    int16_t get_raw_value(uint8_t reg) const
    {
        auto result = m_i2c_device->read(reg, BYTES_PER_AXIS);
        int16_t raw = static_cast<int16_t>((result[0] << 8) | result[1]);

        return raw;
    }

    /**
     * @brief Sets config for device.
     *
     * Resets the device and disables sleep mode. Also sets the default sensitivity for the accelerometer and gyroscope.
     *
     * @throws I2cException with the corrsponding esp_err_t return value if something goes wrong
     */
    void set_config()
    {
        m_i2c_device->write({POWER_MGT_REG, DEVICE_RESET});
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
        m_i2c_device->write({POWER_MGT_REG, SLEEP_MODE});
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
        m_i2c_device->write({ACCEL_CONFIG_REG, ACCEL_2G});
        m_i2c_device->write({GYRO_CONFIG_REG, GYRO_250DPS});
    }

    std::unique_ptr<I2cDevice> m_i2c_device;
    std::unique_ptr<MPU6050Mapper> m_mapper;
};

MPU6050::MPU6050(uint8_t address) : IMU(), m_impl{std::make_unique<MPU6050Impl>(address)}
{
}

MPU6050::~MPU6050() = default;

const char *MPU6050::get_name() const noexcept
{
    return "[MPU6050]";
}

float MPU6050::read_ax() const
{
    return m_impl->read_ax();
}

float MPU6050::read_ay() const
{
    return m_impl->read_ay();
}

float MPU6050::read_az() const
{
    return m_impl->read_az();
}

float MPU6050::read_gx() const
{
    return m_impl->read_gx();
}

float MPU6050::read_gy() const
{
    return m_impl->read_gy();
}

float MPU6050::read_gz() const
{
    return m_impl->read_gz();
}

} // namespace kopter