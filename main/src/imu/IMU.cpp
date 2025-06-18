#include "pch.hpp"
#include "IMU.hpp"

namespace kopter {
IMU::IMU(const std::string &name) noexcept : Device(std::move(name))
{
}

} // namespace kopter