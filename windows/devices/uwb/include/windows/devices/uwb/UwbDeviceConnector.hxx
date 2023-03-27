
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
#include <windows/devices/uwb/IUwbSessionDdi.hxx>

namespace windows::devices::uwb
{
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
     * @brief Start listening for notifications.
     *
     * @return true If listening for notifications started successfully.
     * @return false If listening for notifications could not be started.
     */
    virtual bool
    NotificationListenerStart() override;

    /**
     * @brief Stop listening for notifications.
     */
    virtual void
    NotificationListenerStop() override;

    /**
     * @brief Sets the callbacks for the UwbDevice that owns this UwbConnector
     *
     * @param callbacks
     * @return RegisteredCallbackToken* You can pass this pointer into DeregisterEventCallback to deregister this event callback
     */
    virtual ::uwb::RegisteredCallbackToken*
    RegisterDeviceEventCallbacks(std::weak_ptr<::uwb::UwbRegisteredDeviceEventCallbacks> callbacks) override;

public:
    // IUwbSessionDdiConnector
    /**
     * @brief Registers the callbacks for a particular session
     *
     * @param sessionId
     * @param callbacks
     * @return RegisteredCallbackToken* You can pass this pointer into DeregisterEventCallback to deregister this event callback
     */
    virtual ::uwb::RegisteredCallbackToken*
    RegisterSessionEventCallbacks(uint32_t sessionId, std::weak_ptr<::uwb::UwbRegisteredSessionEventCallbacks> callbacks) override;

public:
    /**
     * @brief De-registers the callback associated with the token
     * If you pass in a token that is no longer valid, this function does nothing
     *
     * @param token
     */
    void
    DeregisterEventCallback(::uwb::RegisteredCallbackToken* token);

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

public:
    // IUwbSessionDdi
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

    virtual std::future<std::tuple<::uwb::protocol::fira::UwbStatus, std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameter>>>
    GetApplicationConfigurationParameters(uint32_t sessionId, std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameterType> applicationConfigurationParameterTypes) override;

    virtual std::future<std::tuple<::uwb::protocol::fira::UwbStatus, std::vector<::uwb::protocol::fira::UwbSetApplicationConfigurationParameterStatus>>>
    SetApplicationConfigurationParameters(uint32_t sessionId, std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameter> applicationConfigurationParameters) override;

private:
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
    wil::shared_hfile m_notificationHandleDriver;
    OVERLAPPED m_notificationOverlapped;
};
} // namespace windows::devices::uwb

#endif // UWB_DEVICE_CONNECTOR_HXX
