#pragma once

#include <functional>
#include <memory>

namespace idf::event {

class ESPEventLoop;
class ESPEventReg;
class ESPEvent;

} // namespace idf::event

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

    std::unique_ptr<idf::event::ESPEventReg> register_system_event(
        const idf::event::ESPEvent &event, std::function<void(const idf::event::ESPEvent &, void *)> cb
    );

private:
    LoopManager();

private:
    std::shared_ptr<idf::event::ESPEventLoop> m_default_loop;
    std::shared_ptr<idf::event::ESPEventLoop> m_custom_loop;
};

} // namespace kopter