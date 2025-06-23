#include "pch.hpp"
#include "PIDException.hpp"

namespace kopter {
PIDException::PIDException(esp_err_t error) : KopterException(error)
{
}
} // namespace kopter