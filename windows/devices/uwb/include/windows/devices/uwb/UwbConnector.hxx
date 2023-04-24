
#ifndef UWB_DEVICE_CONNECTOR_HXX
#define UWB_DEVICE_CONNECTOR_HXX

#include <cstdint>
#include <future>
#include <memory>
#include <optional>
#include <shared_mutex>
#include <string>
#include <thread>
#include <tuple>
#include <unordered_map>
#include <vector>

#include <uwb/UwbRegisteredCallbacks.hxx>
#include <uwb/UwbSessionEventCallbacks.hxx>
#include <uwb/protocols/fira/UwbCapability.hxx>
#include <wil/resource.h>
#include <windows/devices/uwb/IUwbDeviceDdi.hxx>
#include <windows/devices/uwb/IUwbSessionDdi.hxx>

namespace uwb
{
/**
 * @brief The following are opaque class declarations that are used in this file
 *
 */
class RegisteredSessionCallbackToken;
class OnSessionEndedToken;
class OnRangingStartedToken;
class OnRangingStoppedToken;
class OnPeerPropertiesChangedToken;
class OnSessionMembershipChangedToken;

class RegisteredDeviceCallbackToken;
class OnStatusChangedToken;
class OnDeviceStatusChangedToken;
class OnSessionStatusChangedToken;

} // namespace uwb

namespace windows::devices::uwb
{
/**
 * @brief Class representing a logical communication channel with a UWB driver.
 *
 * This class exposes functions which map 1-1 to the UWB LRP DDI as defined in
 * UwbCxLrpDevice.h, using neutral C++ types instead of the the raw C ABI
 * types.
 */
class UwbConnector :
    public IUwbDeviceDdiConnector,
    public IUwbSessionDdiConnector
{
public:
    /**
     * @brief Construct a new UwbConnector object.
     *
     * @param deviceName The interface path name.
     */
    explicit UwbConnector(std::string deviceName);

    /**
     * @brief Destroy the Uwb Device Connector object
     */
    ~UwbConnector();

    /**
     * @brief Get the name of this device.
     *
     * @return const std::string&
     */
    const std::string&
    DeviceName() const noexcept;

public:
    // IUwbDeviceDdiConnector
    /**
     * @brief Sets the callbacks for the UwbDevice that owns this UwbConnector
     * You can pass in a partially filled in struct, in which case this function returns a partially filled in struct of tokens corresponding to those callbacks.
     *
     * @param callbacks
     * @return ::uwb::UwbRegisteredDeviceEventCallbackTokens You can pass the pointers into DeregisterEventCallback to deregister the event callbacks
     */
    virtual ::uwb::UwbRegisteredDeviceEventCallbackTokens
    RegisterDeviceEventCallbacks(::uwb::UwbRegisteredDeviceEventCallbacks callbacks) override;

public:
    // IUwbSessionDdiConnector
    /**
     * @brief Registers the callbacks for a particular session.
     * You can pass in a partially filled in struct, in which case this function returns a partially filled in struct of tokens corresponding to those callbacks.
     *
     * @param sessionId
     * @param callbacks
     * @return ::uwb::UwbRegisteredSessionEventCallbackTokens You can pass the pointers into DeregisterEventCallback to deregister the event callbacks
     */
    virtual ::uwb::UwbRegisteredSessionEventCallbackTokens
    RegisterSessionEventCallbacks(uint32_t sessionId, ::uwb::UwbRegisteredSessionEventCallbacks callbacks) override;

public:
    /**
     * @brief De-registers the callback associated with the token
     * If you pass in a token that is no longer valid, this function does nothing
     *
     * @param token
     */
    void
    DeregisterEventCallback(std::weak_ptr<::uwb::RegisteredCallbackToken> token);

public:
    // IUwbDeviceDdi
    virtual std::future<void>
    Reset() override;

    virtual std::future<::uwb::protocol::fira::UwbDeviceInformation>
    GetDeviceInformation() override;

    virtual std::future<std::tuple<::uwb::protocol::fira::UwbStatus, ::uwb::protocol::fira::UwbCapability>>
    GetCapabilities() override;

    virtual std::future<std::tuple<::uwb::protocol::fira::UwbStatus, uint32_t>>
    GetSessionCount() override;

public:
    // IUwbSessionDdi
    virtual std::future<::uwb::protocol::fira::UwbStatus>
    SessionInitialize(uint32_t sessionId, ::uwb::protocol::fira::UwbSessionType sessionType) override;

    virtual std::future<::uwb::protocol::fira::UwbStatus>
    SessionDeinitialize(uint32_t sessionId) override;

    virtual std::future<std::tuple<::uwb::protocol::fira::UwbStatus, ::uwb::protocol::fira::UwbSessionState>>
    SessionGetState(uint32_t sessionId) override;

    virtual std::future<::uwb::protocol::fira::UwbStatus>
    SessionRangingStart(uint32_t sessionId) override;

    virtual std::future<::uwb::protocol::fira::UwbStatus>
    SessionRangingStop(uint32_t sessionId) override;

    virtual std::future<std::tuple<::uwb::protocol::fira::UwbStatus, std::optional<uint32_t>>>
    SessionGetRangingCount(uint32_t sessionId) override;

    virtual std::future<::uwb::protocol::fira::UwbStatus>
    SessionUpdateControllerMulticastList(uint32_t sessionId, ::uwb::protocol::fira::UwbMulticastAction multicastAction, std::vector<::uwb::UwbMacAddress> controlees) override;

    virtual std::future<std::tuple<::uwb::protocol::fira::UwbStatus, std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameter>>>
    GetApplicationConfigurationParameters(uint32_t sessionId, std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameterType> applicationConfigurationParameterTypes) override;

    virtual std::future<std::tuple<::uwb::protocol::fira::UwbStatus, std::vector<::uwb::protocol::fira::UwbSetApplicationConfigurationParameterStatus>>>
    SetApplicationConfigurationParameters(uint32_t sessionId, std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameter> applicationConfigurationParameters) override;

private:
    /**
     * @brief Start listening for notifications.
     *
     * @return true If listening for notifications started successfully.
     * @return false If listening for notifications could not be started.
     */
    void
    NotificationListenerStart();

    /**
     * @brief Stop listening for notifications.
     */
    void
    NotificationListenerStop();

    /**
     * @brief Thread function for handling UWB notifications from the driver.
     *
     * @param stopToken The token used to request the notification loop to stop.
     * @param onNotification The callback function to invoke for each notification.
     */
    void
    HandleNotifications(std::stop_token stopToken);

    /**
     * @brief Responsible for calling the relevant registered callbacks for the uwbNotificationData
     *
     * @param uwbNotificationData
     */
    void
    DispatchCallbacks(::uwb::protocol::fira::UwbNotificationData uwbNotificationData);

    /**
     * @brief Response for calling the relevant registered callbacks for the session ended event.
     * This function assumes the caller is holding the m_eventCallbacksGate
     *
     * @param sessionId The session identifier of the session that ended.
     * @param sessionEndReason The reason the session ended.
     */
    void
    OnSessionEnded(uint32_t sessionId, ::uwb::UwbSessionEndReason sessionEndReason);

    /**
     * @brief Internal function that prepares the notification for processing by the m_sessionEventCallbacks
     * This function assumes the caller is holding the m_eventCallbacksGate
     * @param statusMulticastList
     */
    void
    OnSessionMulticastListStatus(::uwb::protocol::fira::UwbSessionUpdateMulticastListStatus statusMulticastList);

    /**
     * @brief Internal function that prepares the notification for processing by the m_sessionEventCallbacks
     * This function assumes the caller is holding the m_eventCallbacksGate
     *
     * @param rangingData
     */
    void
    OnSessionRangingData(::uwb::protocol::fira::UwbRangingData rangingData);

    /**
     * @brief Internal function to check if there are callbacks present.
     * This function assumes the caller is holding the m_eventCallbacksGate
     *
     * @return true
     * @return false
     */
    bool
    CallbacksPresent();

    // Internal function that wraps DeviceIoControl, handles errors and insufficient buffer size
    void
    DeviceIoControlWrapper();

private:
    std::string m_deviceName{};
    std::jthread m_notificationThread;
    wil::shared_hfile m_notificationHandleDriver;
    OVERLAPPED m_notificationOverlapped;

    // the following shared_mutex is used to protect access to everything regarding the registered callbacks
    mutable std::shared_mutex m_eventCallbacksGate;
    std::unordered_map<uint32_t, std::vector<std::shared_ptr<::uwb::OnSessionEndedToken>>> m_onSessionEndedCallbacks;
    std::unordered_map<uint32_t, std::vector<std::shared_ptr<::uwb::OnRangingStartedToken>>> m_onRangingStartedCallbacks;
    std::unordered_map<uint32_t, std::vector<std::shared_ptr<::uwb::OnRangingStoppedToken>>> m_onRangingStoppedCallbacks;
    std::unordered_map<uint32_t, std::vector<std::shared_ptr<::uwb::OnPeerPropertiesChangedToken>>> m_onPeerPropertiesChangedCallbacks;
    std::unordered_map<uint32_t, std::vector<std::shared_ptr<::uwb::OnSessionMembershipChangedToken>>> m_onSessionMembershipChangedCallbacks;

    std::vector<std::shared_ptr<::uwb::OnStatusChangedToken>> m_onStatusChangedCallbacks;
    std::vector<std::shared_ptr<::uwb::OnDeviceStatusChangedToken>> m_onDeviceStatusChangedCallbacks;
    std::vector<std::shared_ptr<::uwb::OnSessionStatusChangedToken>> m_onSessionStatusChangedCallbacks;
};
} // namespace windows::devices::uwb

#endif // UWB_DEVICE_CONNECTOR_HXX
