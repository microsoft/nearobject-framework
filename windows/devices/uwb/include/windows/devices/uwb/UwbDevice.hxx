
#ifndef WINDOWS_DEVICE_UWB_HXX
#define WINDOWS_DEVICE_UWB_HXX

#include <concepts>
#include <memory>
#include <string>
#include <thread>
#include <type_traits>

// NB: This must come before any other Windows include
#include <windows.h>

#include <cfgmgr32.h>
#include <wil/resource.h>

#include <uwb/UwbDevice.hxx>
#include <uwb/UwbSession.hxx>
#include <uwb/protocols/fira/FiraDevice.hxx>
#include <windows/devices/DeviceResource.hxx>
#include <windows/devices/uwb/UwbDeviceConnector.hxx>

namespace uwb
{
class UwbSession;
}

namespace windows::devices::uwb
{
/**
 * @brief Helper class to interact with Windows UWB devices using the Windows
 * UWB DDI.
 */
class UwbDevice :
    public ::uwb::UwbDevice
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
     * @brief Determine if this device is the same as another.
     *
     * @param other
     * @return true
     * @return false
     */
    bool
    IsEqual(const ::uwb::UwbDevice& other) const noexcept override;

private:
    /**
     * @brief Create a new UWB session.
     *
     * @param callbacks The event callback instance.
     * @return std::shared_ptr<uwb::UwbSession>
     */
    virtual std::shared_ptr<::uwb::UwbSession>
    CreateSessionImpl(std::weak_ptr<::uwb::UwbSessionEventCallbacks> callbacks) override;

    /**
     * @brief Get the capabilities of the device.
     *
     * @return uwb::protocol::fira::UwbCapability
     */
    virtual ::uwb::protocol::fira::UwbCapability
    GetCapabilitiesImpl() override;

    /**
     * @brief Get the FiRa device information of the device.
     * 
     * @return ::uwb::protocol::fira::UwbDeviceInformation 
     */
    virtual ::uwb::protocol::fira::UwbDeviceInformation
    GetDeviceInformationImpl() override;

private:
    const std::string m_deviceName;
    std::shared_ptr<UwbDeviceConnector> m_uwbDeviceConnector;
};
} // namespace windows::devices::uwb

#endif // WINDOWS_DEVICE_UWB_HXX
