
#ifndef UWB_DEVICE_SIMULATOR_HXX
#define UWB_DEVICE_SIMULATOR_HXX

#include <cstdint>
#include <string>

// NB: This must come before any other Windows include
#include <windows.h>

#include <cfgmgr32.h>
#include <windows/devices/DeviceResource.hxx>
#include <wil/resource.h>

namespace windows::devices::uwb::simulator
{
class UwbDeviceSimulator
{
public:
    /**
     * @brief Construct a new UwbDeviceSimulator object.
     * 
     * @param deviceName The device name (path).
     */
    explicit UwbDeviceSimulator(std::string deviceName);

    /**
     * @brief Get the name of this device.
     *
     * @return const std::string&
     */
    const std::string&
    DeviceName() const noexcept;

    /**
     * @brief Initialize the device.
     * TODO: return type needs to convey whether this worked.
     */
    void
    Initialize();

private:
    const std::string m_deviceName;

    unique_hcmnotification m_hcmNotificationHandle;
    wil::unique_hfile m_handleDriver;
};

} // namespace windows::devices::uwb::simulator

#endif // UWB_DEVICE_SIMULATOR_HXX
