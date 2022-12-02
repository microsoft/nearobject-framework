
#ifndef WINDOWS_DEVICE_UWB_HXX
#define WINDOWS_DEVICE_UWB_HXX

#include <memory>
#include <string>

// NB: This must come before any other Windows include
#include <windows.h>

#include <cfgmgr32.h>
#include <wil/resource.h>

#include <uwb/UwbDevice.hxx>
#include <uwb/UwbSessionEventCallbacks.hxx>
#include <windows/devices/DeviceResource.hxx>

namespace uwb
{
class UwbSession;
}

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
    explicit UwbDevice(std::string deviceName);

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

    /**
     * @brief Create a new UWB session.
     * 
     * @param sessionId The unique session identifier to use.
     * @param callbacks The event callback instance.
     * @return std::unique_ptr<uwb::UwbSession> 
     */
    std::unique_ptr<uwb::UwbSession>
    CreateSession(uint32_t sessionId, std::weak_ptr<uwb::UwbSessionEventCallbacks> callbacks) override;

    /**
     * @brief Get the capabilities of the device.
     * 
     * @return uwb::protocol::fira::UwbCapability 
     */
    uwb::protocol::fira::UwbCapability
    GetCapabilities() const override;

    /**
     * @brief Determine if this device is the same as another.
     * 
     * @param other 
     * @return true 
     * @return false 
     */
    bool
    IsEqual(const uwb::UwbDevice& other) const noexcept override;

private:
    const std::string m_deviceName;

    unique_hcmnotification m_hcmNotificationHandle;
    wil::unique_hfile m_handleDriver;
};

} // namespace devices
} // namespace windows

#endif // WINDOWS_DEVICE_UWB_HXX
