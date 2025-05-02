#pragma once

#include <memory>

#include "esp_event_api.hpp"
#include "esp_event_cxx.hpp"

namespace kopter {

class LoopManager {
public:
    LoopManager(const LoopManager &) = delete;
    LoopManager &operator=(const LoopManager &) = delete;

    static LoopManager &get_instance()
    {
        static LoopManager instance;
        return instance;
    }

    idf::event::ESPEventLoop *get_default_loop() const
    {
        return m_default_loop.get();
    }

    idf::event::ESPEventLoop *get_custom_loop() const
    {
        return m_custom_loop.get();
    }

private:
    LoopManager();

private:
    std::shared_ptr<idf::event::ESPEventLoop> m_default_loop;
    std::shared_ptr<idf::event::ESPEventLoop> m_custom_loop;
};

} // namespace kopter