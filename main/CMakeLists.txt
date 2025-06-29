idf_build_get_property(target IDF_TARGET)
if (NOT target)
    set(target "esp32")
    message(WARNING "IDF_TARGET not set, defaulting to esp32")
endif()

set(srcdirs 
        "src"
        "src/core/peripheral/adc"
        "src/core/peripheral/i2c"
        "src/imu"
        "src/imu/filter"
        "src/imu/mpu6050"
        "src/manager"
        "src/motor"
        "src/motor/bdc"
        "src/motor/mixer"
        "src/pid"
        "src/sensor/barometer"
        "src/sensor/barometer/bmp280"
        "src/sensor/imu"
        "src/sensor/imu/filter"
        "src/sensor/imu/mpu6050"
)
set(includedirs 
        "include"
        "include/core"
        "include/core/exception"
        "include/core/peripheral"
        "include/core/peripheral/adc"
        "include/core/peripheral/i2c"
        "include/core/utils"
        "include/imu"
        "include/imu/filter"
        "include/imu/mpu6050"
        "include/manager"
        "include/motor"
        "include/motor/bdc"
        "include/motor/mixer"
        "include/pid"
        "include/sensor/barometer"
        "include/sensor/barometer/bmp280"
        "include/sensor/imu"
        "include/sensor/imu/filter"
        "include/sensor/imu/mpu6050"
)

idf_component_register(
    SRC_DIRS ${srcdirs}
    INCLUDE_DIRS ${includedirs}
    PRIV_REQUIRES esp_event esp_wifi nvs_flash esp_adc glm
)

target_compile_options(${COMPONENT_LIB} PRIVATE -std=c++20 -fexceptions)
target_precompile_headers(${COMPONENT_LIB} PRIVATE include/pch.hpp)