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
#include "LoopManager.hpp"

#include "esp_event_api.hpp"
#include "esp_event_cxx.hpp"

using namespace idf::event;

constexpr uint16_t QUEUE_SIZE = 64;
constexpr std::string_view TASK_NAME = "event_task";
constexpr uint16_t TASK_PRIORITY = 5;
constexpr uint32_t TASK_STACK_SIZE = 4096;
constexpr uint16_t CORE_ID = 0;

namespace kopter {

static esp_event_loop_args_t make_loop_args()
{
    esp_event_loop_args_t args;
    args.queue_size = QUEUE_SIZE;
    args.task_name = TASK_NAME.data();
    args.task_priority = TASK_PRIORITY;
    args.task_stack_size = TASK_STACK_SIZE;
    args.task_core_id = CORE_ID;

    return args;
}

LoopManager::LoopManager()
    : m_default_loop(std::make_shared<ESPEventLoop>(std::make_shared<ESPEventAPIDefault>())),
      m_custom_loop(std::make_shared<ESPEventLoop>(std::make_shared<ESPEventAPICustom>(make_loop_args())))
{
}

std::unique_ptr<ESPEventReg> LoopManager::register_system_event(const ESPEvent &event,
                                                                std::function<void(const ESPEvent &, void *)> cb)
{
    std::unique_ptr<ESPEventReg> system_event = m_default_loop->register_event(event, cb);
    return system_event;
}

std::unique_ptr<idf::event::ESPEventReg> LoopManager::register_event(
    const idf::event::ESPEvent &event, std::function<void(const idf::event::ESPEvent &, void *)> cb)
{
    std::unique_ptr<ESPEventReg> system_event = m_custom_loop->register_event(event, cb);
    return system_event;
}

} // namespace kopter