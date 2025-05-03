#ifndef WIFI_MANAGER_HPP
#define WIFI_MANAGER_HPP

#include <memory>

namespace idf::event {

class ESPEventReg;

}

namespace kopter {

class LoopManager;

class WiFiManager {
public:
    WiFiManager(const WiFiManager &) = delete;
    WiFiManager &operator=(const WiFiManager &) = delete;
    ~WiFiManager();

    static WiFiManager &get_instance(LoopManager *p_loop_manager);

    void init_softap();

private:
    WiFiManager();

    LoopManager *m_loop_manager{nullptr};
    std::unique_ptr<idf::event::ESPEventReg> m_ap_connected_event;
};

} // namespace kopter

#endif