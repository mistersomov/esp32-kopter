#ifndef WIFI_MANAGER_HPP
#define WIFI_MANAGER_HPP

namespace idf::event {

class ESPEventReg;

} // namespace idf::event

namespace kopter {

class LoopManager;

class WiFiManager {
public:
    WiFiManager(const WiFiManager &) = delete;
    WiFiManager &operator=(const WiFiManager &) = delete;
    ~WiFiManager();

    static WiFiManager &get_instance(LoopManager *p_loop_manager);

    void init();

private:
    WiFiManager();

    void init_esp_now();
    void set_wifi_config();

    LoopManager *m_loop_manager{nullptr};
};

} // namespace kopter

#endif