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

namespace kopter {

/**
 * @brief Interface representing a generic device in the system.
 *
 * This interface serves as a base for all identifiable hardware or logical devices
 * used within the system. It defines a minimal contract that allows each device
 * to expose its name, which can be used for logging, debugging, or identification.
 *
 * Implementing this interface allows consistent interaction with various device types.
 */
struct IDevice {
    virtual ~IDevice() = default;

    /**
     * @brief Returns the name of the device.
     *
     * @return A null-terminated C-style string representing the device name.
     *         The returned pointer must remain valid for the lifetime of the device.
     */
    virtual const char *get_name() const noexcept = 0;
};

} // namespace kopter