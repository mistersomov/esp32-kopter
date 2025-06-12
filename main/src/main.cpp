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
#include "AdcDeviceHolder.hpp"
#include "LoopManager.hpp"
#include "Message.hpp"
#include "MessageManager.hpp"
#include "motor/Motor.hpp"
#include "WiFiManager.hpp"

#include "freertos/FreeRTOS.h"
#include "gpio_cxx.hpp"

using namespace kopter;

constexpr std::string_view TAG = "[MAIN]";

extern "C" void app_main(void)
{
    auto &loop_manager = LoopManager::get_instance();
    auto &wifi_manager = WiFiManager::get_instance(&loop_manager);
    //     auto &device_factory = PeripheralDeviceFactory::get_instance();
    std::vector<adc_channel_t> channels{ADC_CHANNEL_3};
    auto controller =
        static_cast<AdcDevice *>(AdcDeviceHolder::get_instance().add_device("CONTROLLER", AdcMode::ONE_SHOT, channels));
    // auto controller1 = static_cast<AdcDevice *>(
    //     AdcDeviceHolder::get_instance().add_device("CONTROLLER_3", AdcMode::CONTINUOUS, channels));
    // auto controller2 = static_cast<AdcDevice *>(
    //     AdcDeviceHolder::get_instance().add_device("CONTROLLER_2", AdcMode::CONTINUOUS, channels));

#if CONFIG_WIFI_MODE_SOFTAP
    auto motor = new Motor{33};
    motor->enable();
    motor->forward();

    MessageManager::get_instance().register_callback(DeviceID::CONTROLLER,
                                                     [motor](const Message &msg) { motor->set_speed(msg.data); });
#else

    uint32_t last_data = 0;
    constexpr uint32_t THRESHOLD = 100;

    Message msg;
    while (true) {
        controller->read([&last_data, &msg, THRESHOLD](int16_t voltage) {
            if (std::abs((int)voltage - (int)last_data) > THRESHOLD) {
                last_data = voltage;

                msg.device_id = DeviceID::CONTROLLER;
                msg.data = voltage;
                ;
                MessageManager::get_instance().send_message(msg);
            }
        });
        vTaskDelay(1);
    }
#endif
}