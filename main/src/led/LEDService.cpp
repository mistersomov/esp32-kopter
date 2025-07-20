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
#include "LEDService.hpp"

#include "LEDException.hpp"

namespace kopter {

namespace {
constexpr uint32_t RMT_HZ_RESOLUTION = 10 * 1000 * 1000; // 10 MHz
constexpr std::string_view BLINK_TASK_NAME = "led_blinking_task";
constexpr uint16_t BLINK_TASK_STACK_SIZE = 4096;
constexpr uint16_t BLINK_TASK_STOP_DELAY = 50;
constexpr RGBColor COLORS[] = {
    RGBColor(0, 0, 0),       // OFF
    RGBColor(127, 127, 127), // default / white-ish
    RGBColor(127, 0, 0),     // RED
    RGBColor(127, 127, 0),   // YELLOW
    RGBColor(0, 127, 0)      // GREEN
};
constexpr std::string_view TAG = "[LEDService]";
} // namespace

LEDService::LEDService() : m_led_strip_handler{nullptr}, m_num_leds{0}
{
}

RGBColor LEDService::parse_color(LEDColor color) noexcept
{
    switch (color) {
    case LEDColor::RED:
        return COLORS[2];
    case LEDColor::YELLOW:
        return COLORS[3];
    case LEDColor::GREEN:
        return COLORS[4];
    default:
        return COLORS[1];
    }
}

void LEDService::fill_strip(const RGBColor &color)
{
    for (size_t i = 0; i != m_num_leds; i++) {
        check_call<LEDException>(led_strip_set_pixel(m_led_strip_handler, i, color.r, color.g, color.b));
    }
    check_call<LEDException>(led_strip_refresh(m_led_strip_handler));
}

LEDService::~LEDService()
{
    if (m_led_strip_handler) {
        off();
        check_call<LEDException>(led_strip_del(m_led_strip_handler));
    }
}

LEDService &LEDService::get_instance()
{
    static LEDService instance;
    return instance;
}

void LEDService::configure_led(gpio_num_t gpio, size_t num_leds, led_model_t led_type)
{
    if (m_led_strip_handler) {
        ESP_LOGW(TAG.data(), "configure_led called more than once. Ignored.");
        return;
    }

    m_num_leds = num_leds;

    led_strip_config_t cfg{};
    cfg.strip_gpio_num = gpio;
    cfg.max_leds = num_leds;
    cfg.color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB;
    cfg.led_model = led_type;
    cfg.flags.invert_out = false;

    led_strip_rmt_config_t rmt_cfg{};
    rmt_cfg.clk_src = RMT_CLK_SRC_DEFAULT;
    rmt_cfg.resolution_hz = RMT_HZ_RESOLUTION;
    rmt_cfg.flags.with_dma = false;

    check_call<LEDException>(led_strip_new_rmt_device(&cfg, &rmt_cfg, &m_led_strip_handler));
}

void LEDService::blink(LEDColor color, uint32_t duration)
{
    stop_blink();

    m_blink_running = true;
    m_blink_task.emplace(BLINK_TASK_NAME.data(), BLINK_TASK_STACK_SIZE, [this, color, duration]() {
        while (m_blink_running.load(std::memory_order_acquire)) {
            blink_once(color, duration);
        }
    });
}

void LEDService::stop_blink()
{
    if (!m_blink_task.has_value()) {
        return;
    }

    m_blink_running = false;

    vTaskDelay(pdMS_TO_TICKS(BLINK_TASK_STOP_DELAY));
    off();
}

void LEDService::blink_once(LEDColor color, uint32_t duration)
{
    if (m_led_strip_handler == nullptr) {
        throw LEDException(ESP_ERR_INVALID_STATE);
    }

    const auto rgb_color = parse_color(color);

    fill_strip(rgb_color);
    vTaskDelay(pdMS_TO_TICKS(duration));
    fill_strip(COLORS[0]);
    vTaskDelay(pdMS_TO_TICKS(duration));
    off();
}

void LEDService::off()
{
    if (m_led_strip_handler) {
        check_call<LEDException>(led_strip_clear(m_led_strip_handler));
    }
}

} // namespace kopter