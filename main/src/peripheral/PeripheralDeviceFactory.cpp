#include "pch.hpp"
#include "peripheral/PeripheralDeviceFactory.hpp"

#define ADC_CONV_MODE ADC_CONV_SINGLE_UNIT_1
#define ADC_UNIT ADC_UNIT_1
#define ADC_OUTPUT_FORMAT ADC_DIGI_OUTPUT_FORMAT_TYPE1
#define ADC_ATTENUATION ADC_ATTEN_DB_12
#define ADC_BIT_WIDTH SOC_ADC_DIGI_MAX_BITWIDTH

namespace kopter {

static constexpr uint16_t SAMPLE_FREQUENCY = 20000;
static constexpr uint16_t MAX_STORE_BUF_SIZE = 1024;
static constexpr uint8_t CONV_FRAME_SIZE = 64;

PeripheralDeviceFactory::PeripheralDeviceFactory()
{
    configure_continuous_driver();
}

PeripheralDeviceFactory::~PeripheralDeviceFactory()
{
    for (auto &[_, device] : m_devices) {
        device.reset();
    }
    m_devices.clear();

    if (m_continuous_handler) {
        ESP_ERROR_CHECK(adc_continuous_deinit(m_continuous_handler));
    }
}

std::shared_ptr<PeripheralDevice> PeripheralDeviceFactory::create_device(
    DeviceID id, DeviceAdcMode mode, std::vector<adc_channel_t> channels
)
{
    if (m_devices.find(id) != m_devices.end()) {
        return m_devices[id];
    }

    if (mode == DeviceAdcMode::CONTINUOUS) {
        adc_continuous_config_t dig_cfg{
            .pattern_num = channels.size(),
            .sample_freq_hz = SAMPLE_FREQUENCY,
            .conv_mode = ADC_CONV_MODE,
            .format = ADC_OUTPUT_FORMAT,
        };
        std::vector<adc_digi_pattern_config_t> digi_pattern_cfgs;
        digi_pattern_cfgs.reserve(channels.size());
        for (const auto &chnl : channels) {
            digi_pattern_cfgs.emplace_back(
                adc_digi_pattern_config_t{
                    .atten = ADC_ATTENUATION,
                    .channel = static_cast<uint8_t>(chnl),
                    .unit = ADC_UNIT,
                    .bit_width = ADC_BIT_WIDTH
                }
            );
        }
        dig_cfg.adc_pattern = digi_pattern_cfgs.data();
        ESP_ERROR_CHECK(adc_continuous_config(m_continuous_handler, &dig_cfg));

        auto device = std::make_shared<PeripheralDevice>(CONV_FRAME_SIZE, m_continuous_handler);
        m_devices[id] = device;

        return device;
    }
    else {
        return std::make_shared<PeripheralDevice>(CONV_FRAME_SIZE, nullptr);
    }
}

void PeripheralDeviceFactory::start()
{
    ESP_ERROR_CHECK(adc_continuous_start(m_continuous_handler));
}

void PeripheralDeviceFactory::stop()
{
    ESP_ERROR_CHECK(adc_continuous_stop(m_continuous_handler));
}

void PeripheralDeviceFactory::configure_continuous_driver()
{
    if (m_continuous_handler) {
        return;
    }

    adc_continuous_handle_cfg_t adc_cfg{.max_store_buf_size = MAX_STORE_BUF_SIZE, .conv_frame_size = CONV_FRAME_SIZE};
    ESP_ERROR_CHECK(adc_continuous_new_handle(&adc_cfg, &m_continuous_handler));
}

} // namespace kopter