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
#include "BMP280.hpp"

#include "ByteUtils.hpp"
#include "I2cDeviceHolder.hpp"

namespace kopter {

inline static constexpr uint8_t CTRL_MEAS_REG = 0xF4;
inline static constexpr uint8_t CONFIG_REG = 0xF5;
inline static constexpr uint8_t TEMP_UPPER_BYTE = 0xFA;
inline static constexpr uint8_t PRESSURE_UPPER_BYTE = 0xF7;
inline static constexpr uint8_t CALIB_UPPER_BYTE = 0x88;
inline static constexpr uint8_t TEMP_BYTES = 3;
inline static constexpr uint8_t PRESSURE_BYTES = 3;
inline static constexpr uint8_t CALIB_BYTES = 24;

BMP280::BMP280(uint8_t address)
    : IBarometer(), m_mapper{std::make_unique<BMP280Mapper>()}, m_calib{std::make_unique<BMP280Calibration>()}
{
    m_i2c_device.reset(I2cDeviceHolder::get_instance().add_device("BMP280", address));
    assert(m_i2c_device);
    set_ctrl_meas();
    set_config();
    set_calib_data();
}

const char *BMP280::get_name() const noexcept
{
    return "[BMP280]";
}

float BMP280::read_temperature()
{
    auto result = m_i2c_device->read(TEMP_UPPER_BYTE, TEMP_BYTES);
    int32_t raw = ((static_cast<int32_t>(result[0]) << 16) | (static_cast<int32_t>(result[1]) << 8) | result[2]) >> 4;

    return m_mapper->map_temperature(raw, m_calib.get());
}

float BMP280::read_pressure()
{
    auto result = m_i2c_device->read(PRESSURE_UPPER_BYTE, PRESSURE_BYTES);
    uint32_t raw =
        ((static_cast<uint32_t>(result[0]) << 16) | (static_cast<uint32_t>(result[1]) << 8) | result[2]) >> 4;

    return m_mapper->map_pressure(raw, m_calib.get());
}

float BMP280::read_altitude()
{
    auto temp = read_temperature();
    auto pressure = read_pressure();

    return m_mapper->map_altitude(pressure);
}

void BMP280::set_ctrl_meas()
{
    // Temperature oversampling - Ultra low (x1)
    // Pressure oversampling - Standard resolution (x4)
    // Power mode - Normal
    m_i2c_device->write({CTRL_MEAS_REG, 0x2F});
}

void BMP280::set_config()
{
    // standby - 62.5ms, IIR filter - 4, SPI - off
    m_i2c_device->write({CONFIG_REG, 0x26});
}

void BMP280::set_calib_data()
{
    auto result = m_i2c_device->read(CALIB_UPPER_BYTE, CALIB_BYTES);
    m_calib->dig_T1 = ByteUtils::get_u16(result[0], result[1]);
    m_calib->dig_T2 = ByteUtils::get_s16(result[2], result[3]);
    m_calib->dig_T3 = ByteUtils::get_s16(result[4], result[5]);
    m_calib->dig_P1 = ByteUtils::get_u16(result[6], result[7]);
    m_calib->dig_P2 = ByteUtils::get_s16(result[8], result[9]);
    m_calib->dig_P3 = ByteUtils::get_s16(result[10], result[11]);
    m_calib->dig_P4 = ByteUtils::get_s16(result[12], result[13]);
    m_calib->dig_P5 = ByteUtils::get_s16(result[14], result[15]);
    m_calib->dig_P6 = ByteUtils::get_s16(result[16], result[17]);
    m_calib->dig_P7 = ByteUtils::get_s16(result[18], result[19]);
    m_calib->dig_P8 = ByteUtils::get_s16(result[20], result[21]);
    m_calib->dig_P9 = ByteUtils::get_s16(result[22], result[23]);
}

} // namespace kopter