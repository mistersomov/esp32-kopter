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

#include "I2cException.hpp"
#include "IDevice.hpp"

#include "i2c_cxx.hpp"

namespace kopter {

/**
 * @brief Represents a generic I2C device connected to a shared I2C master.
 *
 * This class provides an abstraction over an I2C device that communicates via a shared I2C master interface.
 * It enables sending and receiving data over I2C using high-level C++ constructs, and can be extended by
 * specific device implementations (e.g. MPU6050, EEPROM, etc.).
 */
class I2cDevice : public IDevice {
public:
    /**
     * @brief Ctor for an I2cDevice with a name, I2C address, and a shared I2C master.
     *
     * @param address I2C address of the device.
     * @param shared_master Pointer to a shared I2CMaster instance responsible for communication.
     */
    I2cDevice(const idf::I2CAddress &address, idf::I2CMaster *shared_master);

    /**
     * @brief Virtual dtor.
     */
    virtual ~I2cDevice() override = default;

    /**
     * @brief Returns the name of the I2cDevice.
     *
     * @return A null-terminated C-style string representing the device name.
     *         The returned pointer must remain valid for the lifetime of the device.
     */
    virtual const char *get_name() const noexcept override;

    /**
     * @brief Writes a buffer of bytes to the I2C device.
     *
     * @param data A vector of bytes to be sent to the device.
     *
     * @throws I2cException If the write operation fails.
     */
    void write(const std::vector<uint8_t> &data);

    /**
     * @brief Reads a sequence of bytes from a specific register.
     *
     * @param reg The register address to read from.
     * @param n_bytes Number of bytes to read.
     * @return A vector containing the read bytes.
     *
     * @throws I2cException If the read operation fails.
     */
    std::vector<uint8_t> read(const uint8_t reg, const uint16_t n_bytes);

    /**
     * @brief Returns the I2C address of the device.
     *
     * @return The I2CAddress instance representing the device's address.
     */
    const idf::I2CAddress &get_address() const noexcept;

private:
    idf::I2CAddress m_address;
    idf::I2CMaster *m_master{nullptr};
};

} // namespace kopter
