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

#include "IAdcReadStrategy.hpp"

namespace kopter {

class OneShotReadStrategy : public IAdcReadStrategy {
public:
    OneShotReadStrategy(adc_oneshot_unit_handle_t shared_handler,
                        adc_cali_handle_t shared_cali_handler,
                        const std::unordered_set<adc_channel_t> &channels);
    ~OneShotReadStrategy() override = default;

    void read(reading_callback cb) override;

private:
    adc_oneshot_unit_handle_t m_one_shot_handler{nullptr};
    adc_cali_handle_t m_cali_handler{nullptr};
    std::unordered_set<adc_channel_t> m_channels;
};

} // namespace kopter
