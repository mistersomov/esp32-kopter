#include "pch.hpp"
#include "I2cException.hpp"

namespace kopter {
I2cException::I2cException(esp_err_t error) : KopterException(error)
{
}
} // namespace kopter