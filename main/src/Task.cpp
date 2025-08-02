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
#include "Task.hpp"

namespace kopter {

namespace {
constexpr uint8_t TASK_PRIORITY_DEFAULT = 5;
} // namespace

Task::Task(const char *task_name, uint32_t stack_size, TaskFn fn) : m_fn(std::move(fn))
{
    xTaskCreate(Task::task_trampoline, task_name, stack_size, this, TASK_PRIORITY_DEFAULT, nullptr);
}

Task::Task(const char *task_name, uint32_t stack_size, UBaseType_t priority, TaskFn fn)
{
}

Task::Task(const char *task_name, uint32_t stack_size, UBaseType_t priority, BaseType_t coreId, TaskFn fn)
    : m_fn(std::move(fn))
{
    xTaskCreatePinnedToCore(Task::task_trampoline, task_name, stack_size, this, priority, nullptr, coreId);
}

void Task::task_trampoline(void *param)
{
    auto *self = static_cast<Task *>(param);
    assert(self);

    self->m_fn();
    vTaskDelete(nullptr);
}

} // namespace kopter