
#ifndef UWB_DEVICE_CONNECTOR_HXX
#define UWB_DEVICE_CONNECTOR_HXX

#include <future>
#include <memory>
#include <optional>
#include <string>
#include <thread>
#include <tuple>
#include <vector>

#include <wil/resource.h>

#include <uwb/protocols/fira/UwbCapability.hxx>
#include <windows/devices/uwb/IUwbDeviceDdi.hxx>

namespace windows::devices::uwb
{
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
    NotificationListenerStart(std::function<void(::uwb::protocol::fira::UwbNotificationData)> onNotification);

    /**
     * @brief Stop listening for notifications.
     */
    void
    NotificationListenerStop();

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
    SessionIntitialize(uint32_t sessionId, ::uwb::protocol::fira::UwbSessionType sessionType) override;

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
    HandleNotifications(wil::shared_hfile handleDriver, std::stop_token stopToken, std::function<void(::uwb::protocol::fira::UwbNotificationData)> onNotification);

private:
    std::string m_deviceName{};
    std::jthread m_notificationThread;
};
} // namespace windows::devices::uwb

#endif // UWB_DEVICE_CONNECTOR_HXX
