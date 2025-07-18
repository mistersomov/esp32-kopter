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

#pragma once

#include "LEDColor.hpp"
#include "RGBColor.hpp"
#include "Task.hpp"

#include "led_strip.h"
#include "soc/gpio_num.h"

namespace kopter {

/**
 * @brief Singleton class to manage RGB LED strip effects and configuration.
 *
 * The LEDService class encapsulates LED strip setup, color parsing, and blinking behaviors.
 * It supports both one-time blinks and continuous blinking in a separate FreeRTOS task.
 */
class LEDService {
public:
    LEDService(const LEDService &) = delete;
    LEDService &operator=(const LEDService &) = delete;

    /**
     * @brief Dtor. Turns off the LED strip and releases resources.
     *
     * @throws LEDException if releasing the LED strip fails.
     */
    ~LEDService();

    /**
     * @brief Get the singleton instance of the LEDService.
     * @return Reference to the LEDService singleton.
     */
    static LEDService &get_instance();

    /**
     * @brief Configure the LED strip with the specified GPIO and number of LEDs.
     *
     * This method initializes the RMT-based LED strip driver.
     *
     * @param gpio GPIO pin connected to the LED strip data line.
     * @param num_leds Number of LEDs on the strip.
     * @param led_type Type of the LED model (default: WS2812).
     *
     * @throws LEDException if LED strip initialization fails.
     */
    void configure_led(gpio_num_t gpio, size_t num_leds, led_model_t led_type = led_model_t::LED_MODEL_WS2812);

    /**
     * @brief Start blinking the LED strip continuously with the given color and duration.
     *
     * This launches a background FreeRTOS task that repeatedly blinks the LEDs
     * with the specified on/off duration and color. If a blink task is already running,
     * it will be stopped and replaced by a new one with the updated parameters.
     *
     * @param color Color to blink.
     * @param duration Time in milliseconds for each on/off phase.
     *
     * @throws LEDException if any error occurs while updating the LED strip.
     */
    void blink(LEDColor color, uint32_t duration);

    /**
     * @brief Stop the currently running blink task (if any) and turn off the LEDs.
     *
     * The method attempts to stop the task gracefully.
     *
     * @throws LEDException if clearing the LED strip fails.
     */
    void stop_blink();

    /**
     * @brief Perform a single blink (on + off) with the given color and duration.
     *
     * @param color Color to blink.
     * @param duration Time in milliseconds for the on and off phases.
     *
     * @throws LEDException if setting or clearing the LED pixels fails.
     */
    void blink_once(LEDColor color, uint32_t duration);

    /**
     * @brief Immediately turn off all LEDs on the strip.
     *
     * @throws LEDException if clearing the LED strip fails.
     */
    void off();

private:
    LEDService();

    /**
     * @brief Convert a high-level LEDColor enum to an RGBColor value.
     *
     * @param color Logical LED color.
     * @return Corresponding RGB color.
     */
    RGBColor parse_color(LEDColor color) noexcept;

    /**
     * @brief Fill all LEDs on the strip with the given RGB color and refresh the display.
     *
     * @param color RGB color to display.
     *
     * @throws LEDException if updating the LED pixels or refreshing the strip fails.
     */
    void fill_strip(const RGBColor &color);

    led_strip_handle_t m_led_strip_handler;
    std::optional<Task> m_blink_task;
    std::atomic<bool> m_blink_running{false};
    size_t m_num_leds;
};
} // namespace kopter