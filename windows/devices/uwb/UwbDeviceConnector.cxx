
#include <ios>
#include <stdexcept>

#include <plog/Log.h>
#include <wil/result.h>

#include <windows/devices/DeviceHandle.hxx>
#include <windows/devices/uwb/UwbCxAdapterDdiLrp.hxx>
#include <windows/devices/uwb/UwbCxDdiLrp.hxx>
#include <windows/devices/uwb/UwbDeviceConnector.hxx>

using namespace windows::devices::uwb;
using namespace ::uwb::protocol::fira;

/**
 * @brief Namespace alias to reduce typing but preserve clarity regarding DDI
 * conversion.
 */
namespace UwbCxDdi = windows::devices::uwb::ddi::lrp;

UwbDeviceConnector::UwbDeviceConnector(std::string deviceName) :
    m_deviceName(std::move(deviceName))
{}

const std::string&
UwbDeviceConnector::DeviceName() const noexcept
{
    return m_deviceName;
}

std::future<UwbStatus>
UwbDeviceConnector::Reset()
{
    std::promise<UwbStatus> resultPromise{};
    auto resultFuture = resultPromise.get_future();
    // TODO: invoke IOCTL_UWB_DEVICE_RESET

    UwbStatus uwbStatus{}; // TODO: set this to value obtained from IOCTL
    resultPromise.set_value(uwbStatus);

    return resultFuture;
}

std::future<::uwb::protocol::fira::UwbDeviceInformation>
UwbDeviceConnector::GetDeviceInformation()
{
    std::promise<UwbDeviceInformation> resultPromise;
    auto resultFuture = resultPromise.get_future();
    // TODO: invoke IOCTL_UWB_GET_DEVICE_INFO

    return resultFuture;
}

std::future<std::tuple<::uwb::protocol::fira::UwbStatus, ::uwb::protocol::fira::UwbCapability>>
UwbDeviceConnector::GetCapabilities()
{
    std::promise<std::tuple<::uwb::protocol::fira::UwbStatus, ::uwb::protocol::fira::UwbCapability>> resultPromise{};
    auto resultFuture = resultPromise.get_future();

    wil::shared_hfile handleDriver;
    auto hr = OpenDriverHandle(handleDriver, m_deviceName.c_str());
    if (FAILED(hr)) {
        PLOG_ERROR << "failed to obtain driver handle for " << m_deviceName << ", hr=" << hr;
        return resultFuture;
    }

    // Determine the amount of memory required for the UWB_DEVICE_CAPABILITIES from the driver.
    DWORD bytesRequired = 0;
    BOOL ioResult = DeviceIoControl(handleDriver.get(), IOCTL_UWB_GET_DEVICE_CAPABILITIES, nullptr, 0, nullptr, 0, &bytesRequired, nullptr);
    if (!LOG_IF_WIN32_BOOL_FALSE(ioResult)) {
        // TODO: need to do something different here
        HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
        PLOG_ERROR << "error when sending IOCTL_UWB_GET_DEVICE_CAPABILITIES, hr=" << std::showbase << std::hex << hr;
        return resultFuture;
    }

    // Allocate memory for the UWB_DEVICE_CAPABILITIES structure, and pass this to the driver request.
    DWORD uwbCapabilitiesSize = bytesRequired;
    auto uwbDeviceCapabilitiesBuffer = std::make_unique<uint8_t[]>(uwbCapabilitiesSize);
    ioResult = DeviceIoControl(handleDriver.get(), IOCTL_UWB_GET_DEVICE_CAPABILITIES, nullptr, 0, uwbDeviceCapabilitiesBuffer.get(), uwbCapabilitiesSize, &bytesRequired, nullptr);
    if (!ioResult) {
        // TODO: need to do something different here
        HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
        PLOG_ERROR << "error when sending IOCTL_UWB_GET_DEVICE_CAPABILITIES, hr=" << std::showbase << std::hex << hr;
        return resultFuture;
    }

    const UWB_DEVICE_CAPABILITIES& uwbDeviceCapabilities = *reinterpret_cast<UWB_DEVICE_CAPABILITIES*>(uwbDeviceCapabilitiesBuffer.get());
    auto deviceCapabilities = UwbCxDdi::To(uwbDeviceCapabilities);
    auto uwbStatus = UwbCxDdi::To(uwbDeviceCapabilities.status);
    auto result = std::make_tuple(uwbStatus, std::move(deviceCapabilities));
    resultPromise.set_value(std::move(result));

    return resultFuture;
}

std::future<std::tuple<::uwb::protocol::fira::UwbStatus, std::optional<uint32_t>>>
UwbDeviceConnector::GetSessionCount()
{
    std::promise<std::tuple<::uwb::protocol::fira::UwbStatus, std::optional<uint32_t>>> resultPromise;
    auto resultFuture = resultPromise.get_future();
    // TODO: invoke IOCTL_UWB_GET_SESSION_COUNT

    return resultFuture;
}

std::future<UwbStatus>
UwbDeviceConnector::SessionIntitialize(uint32_t sessionId, UwbSessionType sessionType)
{
    std::promise<UwbStatus> resultPromise;
    auto resultFuture = resultPromise.get_future();
    // TODO: invoke IOCTL_UWB_SESSION_INIT

    return resultFuture;
}

std::future<UwbStatus>
UwbDeviceConnector::SessionDeinitialize(uint32_t sessionId)
{
    std::promise<UwbStatus> resultPromise;
    auto resultFuture = resultPromise.get_future();
    // TODO: invoke IOCTL_UWB_SESSION_DEINIT

    return resultFuture;
}

std::future<std::tuple<UwbStatus, std::optional<UwbSessionState>>>
UwbDeviceConnector::SessionGetState(uint32_t sessionId)
{
    std::promise<std::tuple<UwbStatus, std::optional<UwbSessionState>>> resultPromise;
    auto resultFuture = resultPromise.get_future();
    // TODO: invoke IOCTL_UWB_GET_SESSION_STATE

    return resultFuture;
}

std::future<UwbStatus>
UwbDeviceConnector::SessionRangingStart(uint32_t sessionId)
{
    std::promise<UwbStatus> resultPromise;
    auto resultFuture = resultPromise.get_future();
    // TODO: invoke IOCTL_UWB_START_RANGING_SESSION

    return resultFuture;
}

std::future<UwbStatus>
UwbDeviceConnector::SessionRangingStop(uint32_t sessionId)
{
    std::promise<UwbStatus> resultPromise;
    auto resultFuture = resultPromise.get_future();
    // TODO: invoke IOCTL_UWB_STOP_RANGING_SESSION

    return resultFuture;
}

std::future<std::tuple<UwbStatus, std::optional<uint32_t>>>
UwbDeviceConnector::SessionGetRangingCount(uint32_t sessionId)
{
    std::promise<std::tuple<UwbStatus, std::optional<uint32_t>>> resultPromise;
    auto resultFuture = resultPromise.get_future();
    // TODO: invoke IOCTL_UWB_GET_RANGING_COUNT

    return resultFuture;
}

std::future<UwbSessionUpdateMulicastListStatus>
UwbDeviceConnector::SessionUpdateControllerMulticastList(uint32_t sessionId, UwbMulticastAction multicastAction, std::vector<::uwb::UwbMacAddress> controlees)
{
    std::promise<UwbSessionUpdateMulicastListStatus> resultPromise;
    auto resultFuture = resultPromise.get_future();
    // TODO: invoke IOCTL_UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST

    return resultFuture;
}

std::future<std::tuple<UwbStatus, std::vector<std::shared_ptr<IUwbAppConfigurationParameter>>>>
UwbDeviceConnector::GetApplicationConfigurationParameters(uint32_t sessionId, std::vector<UwbApplicationConfigurationParameterType> applicationConfigurationParameterTypes)
{
    std::promise<std::tuple<UwbStatus, std::vector<std::shared_ptr<IUwbAppConfigurationParameter>>>> promiseResult;
    auto resultFuture = promiseResult.get_future();
    // TODO: invoke IOCTL_UWB_GET_APP_CONFIG_PARAMS

    return resultFuture;
}

std::future<std::tuple<::uwb::protocol::fira::UwbStatus, std::vector<std::tuple<::uwb::protocol::fira::UwbApplicationConfigurationParameterType, ::uwb::protocol::fira::UwbStatus>>>>
UwbDeviceConnector::SetApplicationConfigurationParameters(uint32_t sessionId, std::vector<std::shared_ptr<IUwbAppConfigurationParameter>> applicationConfigurationParameters)
{
    std::promise<std::tuple<::uwb::protocol::fira::UwbStatus, std::vector<std::tuple<::uwb::protocol::fira::UwbApplicationConfigurationParameterType, ::uwb::protocol::fira::UwbStatus>>>> resultPromise;
    auto resultFuture = resultPromise.get_future();
    // TODO: invoke IOCTL_UWB_SET_APP_CONFIG_PARAMS

    return resultFuture;
}

void
UwbDeviceConnector::HandleNotifications(wil::shared_hfile handleDriver, std::stop_token stopToken, std::function<void(UwbNotificationData)> onNotification)
{
    // TODO: the below loop invokes the IOCTL synchronously, blocking on a
    // result. There is no known way to cancel the blocking call on the client
    // side; as such, even when stop is requested on the stop token, it cannot
    // be evaluated since the blocking call won't be interrupted.
    //
    // The correct solution here is to open the handle in overlapped mode, and
    // make a non-blocking call. This is not trivial, so will be done later.
    while (!stopToken.stop_requested()) {
        // Determine the amount of memory required for the UWB_NOTIFICATION_DATA from the driver.
        DWORD bytesRequired = 0;
        BOOL ioResult = DeviceIoControl(handleDriver.get(), IOCTL_UWB_NOTIFICATION, nullptr, 0, nullptr, 0, &bytesRequired, nullptr);
        if (!LOG_IF_WIN32_BOOL_FALSE(ioResult)) {
            HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
            PLOG_ERROR << "error determining output buffer size for UWB notification, hr=" << std::showbase << std::hex << hr;
            continue;
        }

        // Allocate memory for the UWB_NOTIFICATION_DATA structure, and pass this to the driver request.
        DWORD uwbNotificationDataSize = bytesRequired;
        std::vector<uint8_t> uwbNotificationDataBuffer(uwbNotificationDataSize);
        ioResult = DeviceIoControl(handleDriver.get(), IOCTL_UWB_NOTIFICATION, nullptr, 0, std::data(uwbNotificationDataBuffer), uwbNotificationDataSize, &bytesRequired, nullptr);
        if (!LOG_IF_WIN32_BOOL_FALSE(ioResult)) {
            HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
            PLOG_ERROR << "error obtaining UWB notification buffer, hr=" << std::showbase << std::hex << hr;
            continue;
        }

        // Log a message if the output buffer is not aligned to UWB_NOTIFICATION_DATA. Ignore it for now as this should not occur often.
        if (reinterpret_cast<uintptr_t>(std::data(uwbNotificationDataBuffer)) % alignof(UWB_NOTIFICATION_DATA) != 0) {
            PLOG_ERROR << "driver output buffer is unaligned! undefined behavior may result";
        }

        // Convert to neutral type and process the notification.
        const UWB_NOTIFICATION_DATA& notificationData = *reinterpret_cast<UWB_NOTIFICATION_DATA*>(std::data(uwbNotificationDataBuffer));
        auto uwbNotificationData = UwbCxDdi::To(notificationData);

        // Invoke callback with notification data.
        onNotification(std::move(uwbNotificationData));
    }
}

bool
UwbDeviceConnector::NotificationListenerStart(std::function<void(::uwb::protocol::fira::UwbNotificationData)> onNotification)
{
    wil::shared_hfile handleDriver;
    auto hr = OpenDriverHandle(handleDriver, m_deviceName.c_str());
    if (FAILED(hr)) {
        PLOG_ERROR << "failed to obtain driver handle for " << m_deviceName << ", hr=" << hr;
        return false;
    }

    m_notificationThread = std::jthread([this, handleDriver = std::move(handleDriver), onNotification = std::move(onNotification)](std::stop_token stopToken) {
        HandleNotifications(std::move(handleDriver), stopToken, std::move(onNotification));
    });

    return true;
}

void
UwbDeviceConnector::NotificationListenerStop()
{
    m_notificationThread.request_stop();
}