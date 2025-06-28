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

#include "BMP280Mapper.hpp"

#include <cmath>

namespace kopter {

static constexpr float SEA_LEVEL_PRESSURE = 101325.0f;
static constexpr float ALTITUDE_SCALE = 44330.0f;
static constexpr float ALTITUDE_EXPONENT = 0.1903f;
static constexpr float HPA2PA = 256.0f * 100.0f; // Convert to Pa: raw / 256 gives hPa, multiply by 100 to get Pa

BMP280Mapper::BMP280Mapper() : m_t_fine{0}
{
}

float BMP280Mapper::map_temperature(int32_t raw, BMP280Calibration &calib)
{
    return get_compensated_temperature(raw, calib);
}

float BMP280Mapper::map_pressure(uint32_t raw, BMP280Calibration &calib)
{
    return get_compensated_pressure(raw, calib);
}

float BMP280Mapper::map_altitude(float pressure)
{
    return ALTITUDE_SCALE * (1.0f - std::pow(pressure / SEA_LEVEL_PRESSURE, ALTITUDE_EXPONENT));
}

float BMP280Mapper::get_compensated_temperature(int32_t adc_t, BMP280Calibration &calib)
{
    const int32_t adc_t_shifted_3 = static_cast<int32_t>(adc_t >> 3);
    const int32_t adc_t_shifted_4 = static_cast<int32_t>(adc_t >> 4);
    const int32_t dig_T1 = static_cast<int32_t>(calib.dig_T1);
    const int32_t dig_T2 = static_cast<int32_t>(calib.dig_T2);
    const int32_t dig_T3 = static_cast<int32_t>(calib.dig_T3);

    const int32_t delta_3 = adc_t_shifted_3 - (dig_T1 << 1);
    const int32_t delta_4 = adc_t_shifted_4 - dig_T1;

    const int32_t var1 = (delta_3 * dig_T2) >> 11;
    const int32_t var2 = ((delta_4 * delta_4) >> 12) * dig_T3 >> 14;

    m_t_fine = var1 + var2;

    return static_cast<float>((m_t_fine * 5 + 128) >> 8) / 100.0f;
}

float BMP280Mapper::get_compensated_pressure(uint32_t adc_p, BMP280Calibration &calib)
{
    const int64_t dig_P1 = static_cast<int64_t>(calib.dig_P1);
    const int64_t dig_P2 = static_cast<int64_t>(calib.dig_P2);
    const int64_t dig_P3 = static_cast<int64_t>(calib.dig_P3);
    const int64_t dig_P4 = static_cast<int64_t>(calib.dig_P4);
    const int64_t dig_P5 = static_cast<int64_t>(calib.dig_P5);
    const int64_t dig_P6 = static_cast<int64_t>(calib.dig_P6);
    const int64_t dig_P7 = static_cast<int64_t>(calib.dig_P7);
    const int64_t dig_P8 = static_cast<int64_t>(calib.dig_P8);
    const int64_t dig_P9 = static_cast<int64_t>(calib.dig_P9);

    int64_t var1 = static_cast<int64_t>(m_t_fine) - 128000;
    int64_t var2 = var1 * var1 * dig_P6;
    var2 = var2 + ((var1 * dig_P5) << 17);
    var2 = var2 + (dig_P4 << 35);
    var1 = ((var1 * var1 * dig_P3) >> 8) + ((var1 * dig_P2) << 12);
    var1 = ((static_cast<int64_t>(1) << 47) + var1) * dig_P1 >> 33;

    if (var1 == 0) {
        return 0.0f;
    }

    int64_t pressure = 1048576 - adc_p;
    pressure = ((pressure * 3125) / var1);
    var1 = (dig_P9 * (pressure >> 13) * (pressure >> 13)) >> 25;
    var2 = (dig_P8 * pressure) >> 19;
    pressure = ((pressure + var1 + var2) >> 8) + (dig_P7 << 4);
    pressure = static_cast<uint32_t>(pressure);

    return static_cast<float>(pressure) / HPA2PA;
}

} // namespace kopter