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

#include "esp_event_cxx.hpp"

namespace kopter {

/**
 * @brief Singleton service that encapsulates access to default and custom ESP event loops.
 *
 * Provides functionality to register system/custom events and post them to the respective loop.
 * Custom events are dispatched on a user-defined event loop, while system events use the default loop.
 */
class EventService {
public:
    EventService(const EventService &) = delete;
    EventService &operator=(const EventService &) = delete;

    /**
     * @brief Get the singleton instance of EventService.
     * @return Reference to the singleton EventService.
     */
    static EventService &get_instance();

    /**
     * @brief Get the pointer to the default event loop (usually system loop).
     * @return Pointer to the default ESPEventLoop.
     */
    idf::event::ESPEventLoop *get_default_loop() const;

    /**
     * @brief Get the pointer to the custom user-defined event loop.
     * @return Pointer to the custom ESPEventLoop.
     */
    idf::event::ESPEventLoop *get_custom_loop() const;

    /**
     * @brief Register a callback for a system event on the default event loop.
     * @param event The ESPEvent to register.
     * @param cb Callback function to be invoked on event.
     * @return Unique pointer to the event registration handler.
     */
    std::unique_ptr<idf::event::ESPEventReg> register_system_event(
        const idf::event::ESPEvent &event, std::function<void(const idf::event::ESPEvent &, void *)> cb);

    /**
     * @brief Register a callback for a custom event on the custom event loop.
     * @param event The ESPEvent to register.
     * @param cb Callback function to be invoked on event.
     * @return Unique pointer to the event registration handler.
     */
    std::unique_ptr<idf::event::ESPEventReg> register_event(
        const idf::event::ESPEvent &event, std::function<void(const idf::event::ESPEvent &, void *)> cb);

    /**
     * @brief Post an event without data to the custom event loop.
     * @param event The event to post.
     */
    void post_event(const idf::event::ESPEvent &event);

    /**
     * @brief Post an event with data to the custom event loop.
     * @tparam T Type of the event data.
     * @param event The event to post.
     * @param data Reference to the data to be sent with the event.
     */
    template <typename T> inline void post_event(const idf::event::ESPEvent &event, T &data)
    {
        try {
            m_custom_loop->post_event_data<T>(event, data);
        }
        catch (const idf::ESPException &e) {
            ESP_LOGE("[EventService]",
                     "Failed to post the event %s(%d). Check the event.",
                     event.base,
                     event.id.get_id());
        }
    }

private:
    EventService();

private:
    std::shared_ptr<idf::event::ESPEventLoop> m_default_loop;
    std::shared_ptr<idf::event::ESPEventLoop> m_custom_loop;
};

} // namespace kopter