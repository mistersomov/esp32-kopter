#include "pch.hpp"
#include "MotorException.hpp"

namespace kopter {

MotorException::MotorException(esp_err_t error) : KopterException(error)
{
}

} // namespace kopter