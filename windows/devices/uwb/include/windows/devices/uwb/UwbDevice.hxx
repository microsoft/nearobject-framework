
#ifndef WINDOWS_DEVICE_UWB_HXX
#define WINDOWS_DEVICE_UWB_HXX

#include <memory>
#include <string>
#include <thread>

// NB: This must come before any other Windows include
#include <windows.h>

#include <cfgmgr32.h>
#include <wil/resource.h>

#include <uwb/UwbDevice.hxx>
#include <uwb/UwbSession.hxx>
#include <uwb/protocols/fira/FiraDevice.hxx>
#include <windows/devices/DeviceResource.hxx>

namespace uwb
{
class UwbSession;
}

namespace windows::devices::uwb
{
/**
 * @brief Helper class to interact with Windows UWB devices using the Windows
 * UWB DDI. The DDI is to be determined.
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

private:
    /**
     * @brief Thread function for handling UWB notifications from the driver.
     */
    void
    HandleNotifications();

protected:
    /**
     * @brief Obtain a shared instance of the primary driver handle.
     *
     * @return wil::shared_hfile
     */
    wil::shared_hfile
    DriverHandle() noexcept;

    /**
     * @brief Obtain a shared instance of the notification driver handle.
     *
     * @return wil::shared_hfile
     */
    wil::shared_hfile
    DriverHandleNotifications() noexcept;

private:
    const std::string m_deviceName;

    wil::shared_hfile m_handleDriver;
    wil::shared_hfile m_handleDriverNotifications;
    std::jthread m_notificationThread;
};
} // namespace windows::devices::uwb

#endif // WINDOWS_DEVICE_UWB_HXX
