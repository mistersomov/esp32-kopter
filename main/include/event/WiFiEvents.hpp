#ifndef WIFI_EVENTS_HPP
#define WIFI_EVENTS_HPP

#include "esp_event_cxx.hpp"
#include "esp_wifi.h"

namespace kopter::event {
inline const idf::event::ESPEvent WIFI_STARTED =
#if CONFIG_WIFI_MODE_SOFTAP
    idf::event::ESPEvent(WIFI_EVENT, idf::event::ESPEventID(WIFI_EVENT_AP_START));
#else
    idf::event::ESPEvent(WIFI_EVENT, idf::event::ESPEventID(WIFI_EVENT_STA_START));
#endif
} // namespace kopter::event

#endif