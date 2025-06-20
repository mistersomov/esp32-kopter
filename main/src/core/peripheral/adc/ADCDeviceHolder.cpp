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
#include "ADCDeviceHolder.hpp"

#include "ADCException.hpp"
#include "ContinuousReadStrategy.hpp"
#include "OneShotReadStrategy.hpp"

#include "esp_adc/adc_cali_scheme.h"
#include "esp_assert.h"

namespace kopter {

#define ATTENUATION ADC_ATTEN_DB_12
#define BITWIDTH ADC_BITWIDTH_12

static constexpr uint16_t SAMPLE_FREQUENCY = 20000;
static constexpr uint16_t MAX_STORE_BUF_SIZE = 1024;
static constexpr uint8_t CONV_FRAME_SIZE = 32;
static constexpr std::string_view TAG = "[ADCDeviceHolder]";

ADCDeviceHolder::ADCDeviceHolder()
{
    configure_continuous_driver();
    configure_one_shot_driver();
    configure_calibration();
}

ADCDeviceHolder::~ADCDeviceHolder()
{
    if (m_continuous_handler) {
        stop_polling();
        ADC_CHECK_THROW(adc_continuous_deinit(m_continuous_handler));
    }
    if (m_one_shot_handler) {
        ADC_CHECK_THROW(adc_oneshot_del_unit(m_one_shot_handler));
    }
    if (m_cali_handler) {
#ifdef ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
        ADC_CHECK_THROW(adc_cali_delete_scheme_curve_fitting(m_cali_handler));
#elif defined(ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED)
        ADC_CHECK_THROW(adc_cali_delete_scheme_line_fitting(m_cali_handler));
#endif
    }
}

ADCDeviceHolder &ADCDeviceHolder::get_instance()
{
    static ADCDeviceHolder instance;
    return instance;
}

Device *ADCDeviceHolder::add_device(const std::string &name,
                                    ADCMode mode,
                                    const std::unordered_set<adc_channel_t> &channels)
{
    assert(m_continuous_handler);

    if (m_devices.find(name) != m_devices.end()) { // TODO Apply channel checking and etc.
        return m_devices[name].get();
    }

    std::unique_ptr<IADCReadStrategy> strategy;
    if (mode == ADCMode::CONTINUOUS) {
        add_device_continuous(channels);
        strategy = std::make_unique<ContinuousReadStrategy>(m_continuous_handler, m_cali_handler, CONV_FRAME_SIZE);
        start_polling();
    }
    else {
        add_device_one_shot(channels);
        strategy = std::make_unique<OneShotReadStrategy>(m_one_shot_handler, m_cali_handler, channels);
    }

    m_devices[name] = std::make_unique<ADCDevice>(std::move(name), std::move(strategy));
    return m_devices[name].get();
}

void ADCDeviceHolder::start_polling()
{
    try {
        ADC_CHECK_THROW(adc_continuous_start(m_continuous_handler));
    }
    catch (const ADCException &e) {
    }
}

void ADCDeviceHolder::stop_polling()
{
    ADC_CHECK_THROW(adc_continuous_stop(m_continuous_handler));
}

void ADCDeviceHolder::configure_continuous_driver()
{
    if (m_continuous_handler) {
        return;
    }

    adc_continuous_handle_cfg_t adc_cfg{};
    adc_cfg.max_store_buf_size = MAX_STORE_BUF_SIZE;
    adc_cfg.conv_frame_size = CONV_FRAME_SIZE;
    ADC_CHECK_THROW(adc_continuous_new_handle(&adc_cfg, &m_continuous_handler));
}

void ADCDeviceHolder::configure_one_shot_driver()
{
    adc_oneshot_unit_init_cfg_t adc_one_shot_cfg{};
    adc_one_shot_cfg.unit_id = ADC_UNIT_1;
    adc_one_shot_cfg.ulp_mode = ADC_ULP_MODE_DISABLE;

    ADC_CHECK_THROW(adc_oneshot_new_unit(&adc_one_shot_cfg, &m_one_shot_handler));
}

void ADCDeviceHolder::configure_calibration()
{
    try {
#ifdef ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
        adc_cali_scheme_ver_t scheme = ADC_CALI_SCHEME_VER_CURVE_FITTING;
        if (adc_cali_check_scheme(&scheme) == ESP_OK) {
            ESP_LOGI(TAG.data(), "ADC calibration is using curve fitting scheme");
            configure_curve_fitting_calibration();
            return;
        }
#elif defined(ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED)
        adc_cali_scheme_ver_t scheme = ADC_CALI_SCHEME_VER_LINE_FITTING;
        if (adc_cali_check_scheme(&scheme) == ESP_OK) {
            ESP_LOGI(TAG.data(), "ADC calibration is using line fitting scheme");
            configure_line_fitting_calibration();
            return;
        }
#endif

        ESP_LOGW(TAG.data(), "No ADC calibration scheme supported or valid eFuse data missing");
    }
    catch (const ADCException &e) {
        ESP_LOGE(TAG.data(), "Calibration configuration failed: %s (0x%X)", e.what(), e.error);
        m_cali_handler = nullptr;
    }
}

#ifdef ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
void ADCDeviceHolder::configure_curve_fitting_calibration()
{
    adc_cali_curve_fitting_config_t cali_config{};
    cali_config.unit_id = ADC_UNIT_1;
    cali_config.atten = ATTENUATION;
    cali_config.bitwidth = BITWIDTH;
    ADC_CHECK_THROW(adc_cali_create_scheme_curve_fitting(&cali_config, &m_cali_handler));
}

#elif defined(ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED)
void ADCDeviceHolder::configure_line_fitting_calibration()
{
    adc_cali_line_fitting_config_t cali_config{};
    cali_config.unit_id = ADC_UNIT_1;
    cali_config.atten = ATTENUATION;
    cali_config.bitwidth = BITWIDTH;
    ADC_CHECK_THROW(adc_cali_create_scheme_line_fitting(&cali_config, &m_cali_handler));
}
#endif

void ADCDeviceHolder::add_device_continuous(const std::unordered_set<adc_channel_t> &channels)
{
    adc_continuous_config_t dig_cfg{};
    dig_cfg.pattern_num = channels.size();
    dig_cfg.sample_freq_hz = SAMPLE_FREQUENCY;
    dig_cfg.conv_mode = ADC_CONV_SINGLE_UNIT_1;
    dig_cfg.format = ADC_DIGI_OUTPUT_FORMAT_TYPE1;

    std::vector<adc_digi_pattern_config_t> digi_pattern_cfgs;
    digi_pattern_cfgs.reserve(channels.size());
    for (const auto &chnl : channels) {
        adc_digi_pattern_config_t cfg{};
        cfg.atten = ATTENUATION;
        cfg.channel = static_cast<uint8_t>(chnl);
        cfg.unit = ADC_UNIT_1;
        cfg.bit_width = BITWIDTH;

        digi_pattern_cfgs.emplace_back(cfg);
    }

    dig_cfg.adc_pattern = digi_pattern_cfgs.data();

    try {
        ADC_CHECK_THROW(adc_continuous_config(m_continuous_handler, &dig_cfg));
    }
    catch (const ADCException &e) {
    }
}

void ADCDeviceHolder::add_device_one_shot(const std::unordered_set<adc_channel_t> &channels)
{
    adc_oneshot_chan_cfg_t cfg{};
    cfg.atten = ATTENUATION;
    cfg.bitwidth = BITWIDTH;

    for (const auto &chnl : channels) {
        ADC_CHECK_THROW(adc_oneshot_config_channel(m_one_shot_handler, chnl, &cfg));
    }
}

} // namespace kopter