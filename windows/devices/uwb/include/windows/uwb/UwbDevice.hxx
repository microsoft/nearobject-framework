
#ifndef WINDOWS_DEVICE_UWB_HXX
#define WINDOWS_DEVICE_UWB_HXX

#include <string>

#include <windows.h>
#include <cfgmgr32.h>
#include <wil/resource.h>

#include <uwb/UwbDevice.hxx>
#include <DeviceResource.hxx>

namespace windows
{
namespace devices
{

/**
 * @brief Helper class to interact with Windows UWB devices using the Windows
 * UWB DDI. The DDI is to be determined.
 */
class UwbDevice :
    public uwb::UwbDevice
{
public:
    /**
     * @brief Construct a new Uwb Device object.
     *
     * @param deviceName The interface path name.
     */
    explicit UwbDevice(std::wstring deviceName);

    /**
     * @brief Get the name of this device.
     *
     * @return const std::wstring&
     */
    const std::wstring&
    DeviceName() const noexcept;

    /**
     * @brief Initialize the device. 
     *  TODO: return type needs to convey whether this worked.
     */
    void
    Initialize();

private:
    const std::wstring m_deviceName;

    unique_hcmnotification m_hcmNotificationHandle;
    wil::unique_hfile m_handleDriver;
};

} // namespace devices
} // namespace windows

#endif // WINDOWS_DEVICE_UWB_HXX
