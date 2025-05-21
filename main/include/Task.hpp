#ifndef TASK_HPP
#define TASK_HPP

#include "freertos/FreeRTOS.h"

#include <cstdint>
#include <functional>

namespace kopter {

class Task {
public:
    using TaskFn = std::function<void()>;

    Task(
        const char *task_name, TaskFn fn, uint32_t stack_size = 2048, UBaseType_t priority = 5,
        BaseType_t coreId = tskNO_AFFINITY
    )
        : m_fn(std::move(fn))
    {
        xTaskCreatePinnedToCore(Task::task_trampoline, task_name, stack_size, this, priority, nullptr, coreId);
    }

private:
    static void task_trampoline(void *param)
    {
        auto *self = static_cast<Task *>(param);
        assert(self);

        self->m_fn();
        vTaskDelete(nullptr);
    }

    TaskFn m_fn;
};

} // namespace kopter

#endif