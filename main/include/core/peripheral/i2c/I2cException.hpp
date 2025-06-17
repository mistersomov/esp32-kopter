#pragma once

namespace kopter {

#define I2C_CHECK_THROW(err) CHECK_THROW_WITH(err, I2cException)

struct I2cException : public KopterException {
    I2cException(esp_err_t error);
};
} // namespace kopter