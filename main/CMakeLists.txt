idf_build_get_property(target IDF_TARGET)
if (NOT target)
    set(target "esp32")
    message(WARNING "IDF_TARGET not set, defaulting to esp32")
endif()

set(srcdirs "src" "src/core/peripheral/adc" "src/manager")
set(includedirs "include" "include/core" "include/core/exception" "include/core/peripheral" "include/core/peripheral/adc" "include/manager")

idf_component_register(
    SRC_DIRS ${srcdirs}
    INCLUDE_DIRS ${includedirs}
    PRIV_REQUIRES esp_event esp_wifi nvs_flash esp_adc
)

target_compile_options(${COMPONENT_LIB} PRIVATE -std=c++20 -fexceptions)
target_precompile_headers(${COMPONENT_LIB} PRIVATE include/pch.hpp)