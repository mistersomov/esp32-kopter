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

#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_continuous.h"
#include "esp_adc/adc_oneshot.h"

namespace kopter {

using reading_callback = std::function<void(int16_t voltage, adc_channel_t channel)>;

struct IADCReadStrategy {
    virtual ~IADCReadStrategy() = default;

    virtual void read(reading_callback cb) = 0;
};

} // namespace kopter