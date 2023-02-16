
#ifndef WINDOWS_DEVICE_UWB_HXX
#define WINDOWS_DEVICE_UWB_HXX

#include <memory>
#include <string>
#include <thread>

// NB: This must come before any other Windows include
#include <windows.h>

#include <cfgmgr32.h>
#include <wil/resource.h>

#include <uwb/protocols/fira/FiraDevice.hxx>
#include <uwb/UwbDevice.hxx>
#include <uwb/UwbSessionEventCallbacks.hxx>
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
     * @brief Create a new UWB session.
     *
     * @param callbacks The event callback instance.
     * @return std::unique_ptr<uwb::UwbSession>
     */
    std::unique_ptr<::uwb::UwbSession>
    CreateSession(std::weak_ptr<::uwb::UwbSessionEventCallbacks> callbacks) override;

    /**
     * @brief Get the capabilities of the device.
     *
     * @return uwb::protocol::fira::UwbCapability
     */
    ::uwb::protocol::fira::UwbCapability
    GetCapabilities() const override;

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
     * @brief Invoked when a generic error occurs. 
     * 
     * @param status The generic error that occurred.
     */
    void
    OnStatusChanged(::uwb::protocol::fira::UwbStatus status);

    /**
     * @brief Invoked when the device status changes.
     * 
     * @param statusDevice 
     */
    void 
    OnDeviceStatusChanged(::uwb::protocol::fira::UwbStatusDevice statusDevice);

    /**
     * @brief Invoked when the status of a session changes.
     * 
     * @param statusSession The new status of the session.
     */
    void
    OnSessionStatusChanged(::uwb::protocol::fira::UwbSessionStatus statusSession);

    /**
     * @brief Invoked when the multicast list for a session has a status update.
     * 
     * @param statusMulticastList The status of the session's multicast list.
     */
    void
    OnSessionMulticastListStatus(::uwb::protocol::fira::UwbSessionUpdateMulicastListStatus statusMulticastList);

    /**
     * @brief Invoked when a session has a ranging data update. 
     * 
     * @param rangingData The new ranging data.
     */
    void
    OnSessionRangingData(::uwb::protocol::fira::UwbRangingData rangingData);

    /**
     * @brief Handles a single UWB notification.
     * 
     * @param uwbNotificationData 
     */
    void
    HandleNotification(::uwb::protocol::fira::UwbNotificationData uwbNotificationData);

    /**
     * @brief Thread function for handling UWB notifications from the driver. 
     */
    void
    HandleNotifications();

private:
    const std::string m_deviceName;

    unique_hcmnotification m_hcmNotificationHandle;
    wil::unique_hfile m_handleDriver;
    wil::unique_hfile m_handleDriverNotifications;
    std::jthread m_notificationThread;
};
} // namespace windows::devices::uwb

#endif // WINDOWS_DEVICE_UWB_HXX
