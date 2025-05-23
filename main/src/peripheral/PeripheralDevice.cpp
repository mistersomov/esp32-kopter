#include "pch.hpp"
#include "peripheral/PeripheralDevice.hpp"

#define ADC_UNIT ADC_UNIT_1
#define ADC_CONV_MODE ADC_CONV_SINGLE_UNIT_1
#define ADC_OUTPUT_FORMAT ADC_DIGI_OUTPUT_FORMAT_TYPE1
#define ADC_ATTENUATION ADC_ATTEN_DB_12
#define ADC_BIT_WIDTH SOC_ADC_DIGI_MAX_BITWIDTH

namespace kopter {

static constexpr uint8_t ADC_CHANNEL_NUM = 1;
static constexpr uint16_t MAX_STORE_BUF_SIZE = 1024;
static constexpr uint8_t CONV_FRAME_SIZE = 64;
static constexpr uint16_t SAMPLE_FREQUENCY = 20000;

static constexpr std::string_view TAG = "[PeripheralDevice]";

PeripheralDevice::PeripheralDevice(DeviceAdcMode mode, adc_channel_t channel) : m_channel(channel)
{
    if (mode == DeviceAdcMode::CONTINUOUS) {
        continuous_adc_init();
    }
    else {
    }
}

PeripheralDevice::~PeripheralDevice()
{
    ESP_ERROR_CHECK(adc_continuous_deinit(m_handle));
}

void PeripheralDevice::continuous_adc_init()
{
    adc_continuous_handle_cfg_t adc_cfg{.max_store_buf_size = MAX_STORE_BUF_SIZE, .conv_frame_size = CONV_FRAME_SIZE};
    ESP_ERROR_CHECK(adc_continuous_new_handle(&adc_cfg, &m_handle));

    adc_continuous_config_t dig_cfg = {
        .pattern_num = ADC_CHANNEL_NUM,
        .sample_freq_hz = SAMPLE_FREQUENCY,
        .conv_mode = ADC_CONV_MODE,
        .format = ADC_OUTPUT_FORMAT,
    };

    adc_digi_pattern_config_t
        adc_pattern_cfg{.atten = ADC_ATTENUATION, .channel = m_channel, .unit = ADC_UNIT, .bit_width = ADC_BIT_WIDTH};

    dig_cfg.adc_pattern = &adc_pattern_cfg;

    ESP_ERROR_CHECK(adc_continuous_config(m_handle, &dig_cfg));
    ESP_ERROR_CHECK(adc_continuous_start(m_handle));
}

esp_err_t PeripheralDevice::read_value(uint8_t *buf, uint32_t *out_length)
{
    return adc_continuous_read(m_handle, buf, CONV_FRAME_SIZE, out_length, 0);
}

} // namespace kopter