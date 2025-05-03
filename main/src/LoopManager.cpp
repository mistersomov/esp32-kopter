#include "LoopManager.hpp"
#include "esp_event_api.hpp"
#include "esp_event_cxx.hpp"
#include <string_view>

using namespace idf::event;

constexpr uint16_t QUEUE_SIZE = 64;
constexpr std::string_view TASK_NAME = "event_task";
constexpr uint16_t TASK_PRIORITY = 5;
constexpr uint32_t TASK_STACK_SIZE = 4096;
constexpr uint16_t CORE_ID = 0;

namespace kopter {

static esp_event_loop_args_t make_loop_args()
{
    return esp_event_loop_args_t{
        .queue_size = QUEUE_SIZE,
        .task_name = TASK_NAME.data(),
        .task_priority = TASK_PRIORITY,
        .task_stack_size = TASK_STACK_SIZE,
        .task_core_id = CORE_ID
    };
}

LoopManager::LoopManager()
    : m_default_loop(std::make_shared<ESPEventLoop>(std::make_shared<ESPEventAPIDefault>())),
      m_custom_loop(std::make_shared<ESPEventLoop>(std::make_shared<ESPEventAPICustom>(make_loop_args())))
{
}

std::unique_ptr<ESPEventReg> LoopManager::register_system_event(
    const ESPEvent &event, std::function<void(const ESPEvent &, void *)> cb
)
{
    std::unique_ptr<ESPEventReg> system_event = m_default_loop->register_event(event, cb);
    return system_event;
}

} // namespace kopter