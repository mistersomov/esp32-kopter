idf_build_get_property(target IDF_TARGET)
if (NOT target)
    set(target "esp32")
    message(WARNING "IDF_TARGET not set, defaulting to esp32")
endif()

set(includedirs 
        "include"
        "include/core"
        "include/core/communication"
        "include/core/communication/esp_now"
        "include/core/event"
        "include/core/exception"
        "include/core/firmware"
        "include/core/network"
        "include/core/peripheral"
        "include/core/peripheral/adc"
        "include/core/peripheral/i2c"
        "include/core/utils"
        "include/fc"
        "include/led"
        "include/motor"
        "include/motor/bdc"
        "include/motor/mixer"
        "include/ota"
        "include/pid"
        "include/rc"
        "include/sensor/barometer"
        "include/sensor/barometer/bmp280"
        "include/sensor/imu"
        "include/sensor/imu/filter"
        "include/sensor/imu/mpu6050"
        "include/server"
)
set(srcdirs 
        "src"
        "src/core/communication"
        "src/core/communication/esp_now"
        "src/core/event"
        "src/core/firmware"
        "src/core/network"
        "src/core/peripheral/adc"
        "src/core/peripheral/i2c"
        "src/core/utils"
        "src/fc"
        "src/led"
        "src/motor"
        "src/motor/bdc"
        "src/motor/mixer"
        "src/ota"
        "src/pid"
        "src/rc"
        "src/sensor/barometer"
        "src/sensor/barometer/bmp280"
        "src/sensor/imu"
        "src/sensor/imu/filter"
        "src/sensor/imu/mpu6050"
        "src/server"
)
set(reqs
        esp_adc
        esp_driver_gpio
        esp_driver_ledc
        esp_event
        esp_http_server
        esp_https_ota
        esp_netif
        esp_wifi
        nvs_flash
        glm
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