idf_build_get_property(target IDF_TARGET)
if (NOT target)
    set(target "esp32")
    message(WARNING "IDF_TARGET not set, defaulting to esp32")
endif()

idf_component_register(
    SRC_DIRS src src/manager
    INCLUDE_DIRS include
    PRIV_REQUIRES esp_event esp_wifi nvs_flash
)

target_compile_options(${COMPONENT_LIB} PRIVATE -std=c++20 -fexceptions)