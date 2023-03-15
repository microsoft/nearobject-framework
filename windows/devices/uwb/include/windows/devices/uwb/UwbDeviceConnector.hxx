
#ifndef UWB_DEVICE_CONNECTOR_HXX
#define UWB_DEVICE_CONNECTOR_HXX

#include <cstdint>
#include <future>
#include <memory>
#include <optional>
#include <string>
#include <thread>
#include <tuple>
#include <unordered_map>
#include <vector>

#include <uwb/UwbRegisteredCallbacks.hxx>
#include <uwb/protocols/fira/UwbCapability.hxx>
#include <wil/resource.h>
#include <windows/devices/uwb/IUwbDeviceDdi.hxx>

namespace windows::devices::uwb
{
/**
 * @brief Opaque class forward declaration to help with the deregistration
 *
 */
class RegisteredCallbackToken;

class UwbDeviceConnector :
    public IUwbDeviceDdi
{
public:
    /**
     * @brief Construct a new UwbDeviceConnector object.
     *
     * @param deviceName The interface path name.
     */
    explicit UwbDeviceConnector(std::string deviceName);

    /**
     * @brief Get the name of this device.
     *
     * @return const std::string&
     */
    const std::string&
    DeviceName() const noexcept;

    /**
     * @brief Start listening for notifications.
     *
     * Note: this is a rudimentary implementation and is only present to
     * preserve existing behavior. It will eventually be replaced by a
     * fine-grained publication/subscription model.
     *
     * @param onNotification The handler to invoke for each notification.
     * @return true If listening for notifications started successfully.
     * @return false If listening for notifications could not be started.
     */
    bool
    NotificationListenerStart();

    /**
     * @brief Stop listening for notifications.
     */
    void
    NotificationListenerStop();

    /**
     * @brief Registers the callbacks for a particular session
     *
     * @param sessionId
     * @param callbacks
     * @return RegisteredCallbackToken* You can pass this pointer into DeregisterEventCallback to deregister this event callback
     */
    RegisteredCallbackToken*
    RegisterSessionEventCallbacks(uint32_t sessionId, std::weak_ptr<::uwb::UwbRegisteredSessionEventCallbacks> callbacks);

    /**
     * @brief Sets the callbacks for the UwbDevice that owns this UwbDeviceConnector
     *
     * @param callbacks
     * @return RegisteredCallbackToken* You can pass this pointer into DeregisterEventCallback to deregister this event callback
     */
    RegisteredCallbackToken*
    RegisterDeviceEventCallbacks(std::weak_ptr<::uwb::UwbRegisteredDeviceEventCallbacks> callbacks);

    /**
     * @brief De-registers the callback associated with the 
     * If you pass in a token that is no longer valid, this function does nothing
     *
     * @param token
     */
    void
    DeregisterEventCallback(RegisteredCallbackToken* token);

public:
    // IUwbDeviceDdi
    virtual std::future<void>
    Reset() override;

    virtual std::future<::uwb::protocol::fira::UwbDeviceInformation>
    GetDeviceInformation() override;

    virtual std::future<std::tuple<::uwb::protocol::fira::UwbStatus, ::uwb::protocol::fira::UwbCapability>>
    GetCapabilities() override;

    virtual std::future<std::tuple<::uwb::protocol::fira::UwbStatus, std::optional<uint32_t>>>
    GetSessionCount() override;

    virtual std::future<::uwb::protocol::fira::UwbStatus>
    SessionInitialize(uint32_t sessionId, ::uwb::protocol::fira::UwbSessionType sessionType) override;

    virtual std::future<::uwb::protocol::fira::UwbStatus>
    SessionDeinitialize(uint32_t sessionId) override;

    virtual std::future<std::tuple<::uwb::protocol::fira::UwbStatus, std::optional<::uwb::protocol::fira::UwbSessionState>>>
    SessionGetState(uint32_t sessionId) override;

    virtual std::future<::uwb::protocol::fira::UwbStatus>
    SessionRangingStart(uint32_t sessionId) override;

    virtual std::future<::uwb::protocol::fira::UwbStatus>
    SessionRangingStop(uint32_t sessionId) override;

    virtual std::future<std::tuple<::uwb::protocol::fira::UwbStatus, std::optional<uint32_t>>>
    SessionGetRangingCount(uint32_t sessionId) override;

    virtual std::future<::uwb::protocol::fira::UwbSessionUpdateMulicastListStatus>
    SessionUpdateControllerMulticastList(uint32_t sessionId, ::uwb::protocol::fira::UwbMulticastAction multicastAction, std::vector<::uwb::UwbMacAddress> controlees) override;

    virtual std::future<std::tuple<::uwb::protocol::fira::UwbStatus, std::vector<std::shared_ptr<IUwbAppConfigurationParameter>>>>
    GetApplicationConfigurationParameters(uint32_t sessionId, std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameterType> applicationConfigurationParameterTypes) override;

    virtual std::future<std::tuple<::uwb::protocol::fira::UwbStatus, std::vector<std::tuple<::uwb::protocol::fira::UwbApplicationConfigurationParameterType, ::uwb::protocol::fira::UwbStatus>>>>
    SetApplicationConfigurationParameters(uint32_t sessionId, std::vector<std::shared_ptr<IUwbAppConfigurationParameter>> applicationConfigurationParameters) override;

private:
    /**
     * @brief Thread function for handling UWB notifications from the driver.
     *
     * @param handleDriver The handle to the driver to use for listening for notifications.
     * @param stopToken The token used to request the notification loop to stop.
     * @param onNotification The callback function to invoke for each notification.
     */
    void
    HandleNotifications(wil::shared_hfile handleDriver, std::stop_token stopToken);

    /**
     * @brief Responsible for calling the relevant registered callbacks for the uwbNotificationData
     *
     * @param uwbNotificationData
     */
    void
    DispatchCallbacks(::uwb::protocol::fira::UwbNotificationData uwbNotificationData);

    /**
     * @brief Internal function that prepares the notification for processing by the m_sessionEventCallbacks
     *
     * @param statusMulticastList
     */
    void
    OnSessionMulticastListStatus(::uwb::protocol::fira::UwbSessionUpdateMulicastListStatus statusMulticastList);

    /**
     * @brief Internal function that prepares the notification for processing by the m_sessionEventCallbacks
     *
     * @param rangingData
     */
    void
    OnSessionRangingData(::uwb::protocol::fira::UwbRangingData rangingData);

private:
    std::unordered_map<uint32_t, std::weak_ptr<::uwb::UwbRegisteredSessionEventCallbacks>> m_sessionEventCallbacks;
    std::weak_ptr<::uwb::UwbRegisteredDeviceEventCallbacks> m_deviceEventCallbacks;
    std::string m_deviceName{};
    std::jthread m_notificationThread;
};
} // namespace windows::devices::uwb

#endif // UWB_DEVICE_CONNECTOR_HXX
