#include "esp_exception.hpp"

namespace kopter {

#define CHECK_THROW_WITH(error, exception_type)                                                                        \
    do {                                                                                                               \
        esp_err_t result = (error);                                                                                    \
        if (result != ESP_OK)                                                                                          \
            throw kopter::exception_type(result);                                                                      \
    } while (0)

struct KopterException : public idf::ESPException {
    KopterException(esp_err_t error) : idf::ESPException(error)
    {
    }
};

} // namespace kopter