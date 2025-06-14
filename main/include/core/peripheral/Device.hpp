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

#ifndef DEVICE_HPP
#define DEVICE_HPP

namespace kopter {

class Device {
public:
    explicit Device(const std::string &name) noexcept : m_tag{"[" + name + "]"}, m_name{std::move(name)}
    {
    }
    virtual ~Device() = default;

    const std::string &get_tag() const noexcept
    {
        return m_tag;
    }

private:
    std::string m_tag;
    std::string m_name;
};

} // namespace kopter

#endif