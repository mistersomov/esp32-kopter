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