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
#include "esp_netif_types.h"
#include "esp_wifi_types_generic.h"

namespace kopter {

static const idf::event::ESPEvent IP_GOT_EVENT =
    idf::event::ESPEvent(IP_EVENT, idf::event::ESPEventID(IP_EVENT_STA_GOT_IP));
static const idf::event::ESPEvent STA_START_EVENT =
    idf::event::ESPEvent(WIFI_EVENT, idf::event::ESPEventID(WIFI_EVENT_STA_START));
static const idf::event::ESPEvent STA_CONNECTED_EVENT =
    idf::event::ESPEvent(WIFI_EVENT, idf::event::ESPEventID(WIFI_EVENT_STA_CONNECTED));
static const idf::event::ESPEvent STA_DISCONNECTED_EVENT =
    idf::event::ESPEvent(WIFI_EVENT, idf::event::ESPEventID(WIFI_EVENT_STA_DISCONNECTED));
static const idf::event::ESPEvent SOFT_STA_CONNECTED_EVENT =
    idf::event::ESPEvent(WIFI_EVENT, idf::event::ESPEventID(WIFI_EVENT_AP_STACONNECTED));
static const idf::event::ESPEvent SOFT_STA_DISCONNECTED_EVENT =
    idf::event::ESPEvent(WIFI_EVENT, idf::event::ESPEventID(WIFI_EVENT_AP_STADISCONNECTED));

} // namespace kopter
