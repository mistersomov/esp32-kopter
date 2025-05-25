#ifndef PERIPHERAL_DEVICE_FACTORY_HPP
#define PERIPHERAL_DEVICE_FACTORY_HPP

#include "DeviceAdcMode.hpp"
#include "DeviceID.hpp"
#include "PeripheralDevice.hpp"

namespace kopter {
class PeripheralDeviceFactory {
public:
    PeripheralDeviceFactory(const PeripheralDeviceFactory &) = delete;
    PeripheralDeviceFactory &operator=(const PeripheralDeviceFactory &) = delete;
    ~PeripheralDeviceFactory();

    static PeripheralDeviceFactory &get_instance()
    {
        static PeripheralDeviceFactory instance;
        return instance;
    }

    std::shared_ptr<PeripheralDevice> create_device(
        DeviceID id, DeviceAdcMode mode, std::vector<adc_channel_t> channels
    );
    void start();
    void stop();

private:
    PeripheralDeviceFactory();

    void configure_continuous_driver();

    std::unordered_map<DeviceID, std::shared_ptr<PeripheralDevice>> m_devices;
    adc_continuous_handle_t m_continuous_handler{nullptr};
};
} // namespace kopter

#endif