#pragma once

#include "KopterException.hpp"

namespace kopter {

template <typename Func, class Exception> inline auto wrap_call(Func func) -> decltype(func())
{
    try {
        return func();
    }
    catch (const idf::ESPException &e) {
        throw I2cException(e.error);
    }
}

} // namespace kopter