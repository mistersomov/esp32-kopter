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

#ifndef LOOP_MANAGER_HPP
#define LOOP_MANAGER_HPP

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
        const idf::event::ESPEvent &event, std::function<void(const idf::event::ESPEvent &, void *)> cb);

    std::unique_ptr<idf::event::ESPEventReg> register_event(
        const idf::event::ESPEvent &event, std::function<void(const idf::event::ESPEvent &, void *)> cb);

private:
    LoopManager();

private:
    std::shared_ptr<idf::event::ESPEventLoop> m_default_loop;
    std::shared_ptr<idf::event::ESPEventLoop> m_custom_loop;
};

} // namespace kopter

#endif