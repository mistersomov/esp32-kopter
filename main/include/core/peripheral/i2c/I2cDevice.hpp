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

#include "Device.hpp"
#include "I2cException.hpp"

#include "i2c_cxx.hpp"

namespace kopter {

class I2cDevice : public Device {
public:
    explicit I2cDevice(const std::string &name, const idf::I2CAddress &address, idf::I2CMaster *shared_master) noexcept;
    virtual ~I2cDevice() override = default;

    void write(const std::vector<uint8_t> &data);
    std::vector<uint8_t> read(const uint8_t reg, const uint16_t n_bytes);
    const idf::I2CAddress &get_address() const noexcept;

private:
    template <typename Func> inline auto wrap_i2c(Func func) -> decltype(func())
    {
        try {
            return func();
        }
        catch (const idf::ESPException &e) {
            throw I2cException(e.error);
        }
    }

    idf::I2CAddress m_address;
    idf::I2CMaster *m_master{nullptr};
};

} // namespace kopter
