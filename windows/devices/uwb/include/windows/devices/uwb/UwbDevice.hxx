
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

protected:
    /**
     * @brief Helper function to create a typed (derived) UwbSession object,
     * capturing the common code that is required in all sub-classes.
     *
     * @tparam UwbSessionT The type of session to create.
     * @param callbacks The session callbacks.
     * @return requires
     */
    template <typename UwbSessionT>
    // clang-format off
    requires std::is_base_of_v<::uwb::UwbSession, UwbSessionT>
    // clang-format on
    std::shared_ptr<::uwb::UwbSession>
    CreateSessionImpl(std::weak_ptr<::uwb::UwbSessionEventCallbacks> callbacks)
    {
        // Create a duplicate handle to the driver for use by the session.
        wil::shared_hfile handleDriverForSession;
        if (!DuplicateHandle(GetCurrentProcess(), m_handleDriver.get(), GetCurrentProcess(), &handleDriverForSession, 0, FALSE, DUPLICATE_SAME_ACCESS)) {
            return nullptr;
        }

        return std::make_shared<UwbSessionT>(std::move(callbacks), std::move(handleDriverForSession));
    }

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
    std::shared_ptr<UwbDeviceConnector> m_uwbDeviceConnector;
};
} // namespace windows::devices::uwb

#endif // WINDOWS_DEVICE_UWB_HXX
