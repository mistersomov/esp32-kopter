idf_build_get_property(target IDF_TARGET)
if (NOT target)
    set(target "esp32")
    message(WARNING "IDF_TARGET not set, defaulting to esp32")
endif()

set(includedirs 
        "include"
        "include/core"
        "include/core/exception"
        "include/core/firmware"
        "include/core/network"
        "include/core/peripheral"
        "include/core/peripheral/adc"
        "include/core/peripheral/i2c"
        "include/core/utils"
        "include/fc"
        "include/manager"
        "include/motor"
        "include/motor/bdc"
        "include/motor/mixer"
        "include/ota"
        "include/pid"
        "include/sensor/barometer"
        "include/sensor/barometer/bmp280"
        "include/sensor/imu"
        "include/sensor/imu/filter"
        "include/sensor/imu/mpu6050"
)
set(srcdirs 
        "src"
        "src/core/firmware"
        "src/core/network"
        "src/core/peripheral/adc"
        "src/core/peripheral/i2c"
        "src/fc"
        "src/manager"
        "src/motor"
        "src/motor/bdc"
        "src/motor/mixer"
        "src/ota"
        "src/pid"
        "src/sensor/barometer"
        "src/sensor/barometer/bmp280"
        "src/sensor/imu"
        "src/sensor/imu/filter"
        "src/sensor/imu/mpu6050"
)
set(reqs
        esp_event
        esp_wifi
        nvs_flash
        esp_netif
        esp_adc
        glm
        esp_https_ota
        json
        mbedtls
)

idf_component_register(
    INCLUDE_DIRS ${includedirs}
    SRC_DIRS ${srcdirs}
    PRIV_REQUIRES ${reqs}
)

target_compile_options(${COMPONENT_LIB} PRIVATE -std=c++20 -fexceptions)
target_precompile_headers(${COMPONENT_LIB} PRIVATE include/pch.hpp)