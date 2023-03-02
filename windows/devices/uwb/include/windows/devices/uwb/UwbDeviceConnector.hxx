
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
     * @brief Initialize the device.
     */
    void
    Initialize();

    /**
     * @brief Get the name of this device.
     *
     * @return const std::string&
     */
    const std::string&
    DeviceName() const noexcept;

public:
    // IUwbDeviceDdi
    virtual std::future<::uwb::protocol::fira::UwbStatus>
    Reset() override;

    virtual std::future<::uwb::protocol::fira::UwbDeviceInformation>
    GetDeviceInformation() override;

    virtual std::future<std::tuple<::uwb::protocol::fira::UwbStatus, ::uwb::protocol::fira::UwbCapability>>
    GetCapabilities() override;

    virtual std::future<::uwb::protocol::fira::UwbStatus>
    GetSessionCount(uint32_t &sessionCount) override;

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
    /**
     * @brief Thread function for handling UWB notifications from the driver.
     */
    void
    HandleNotifications();

private:
    std::string m_deviceName{};
    wil::shared_hfile m_handleDriver;
    wil::shared_hfile m_handleDriverNotifications;
    std::jthread m_notificationThread;
};
} // namespace windows::devices::uwb

#endif // UWB_DEVICE_CONNECTOR_HXX
