
#include <ios>
#include <stdexcept>

#include <plog/Log.h>
#include <wil/result.h>

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

wil::shared_hfile
UwbDeviceConnector::DriverHandle() noexcept
{
    return m_handleDriver;
}

wil::shared_hfile
UwbDeviceConnector::DriverHandleNotifications() noexcept
{
    return m_handleDriverNotifications;
}

const std::string&
UwbDeviceConnector::DeviceName() const noexcept
{
    return m_deviceName;
}

void
UwbDeviceConnector::Initialize()
{
    wil::shared_hfile handleDriver(CreateFileA(
        m_deviceName.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED,
        nullptr));

    wil::shared_hfile handleDriverNotifications(CreateFileA(
        m_deviceName.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr));

    // TODO: call CM_Register_Notification(handleDriver, filterType=CM_NOTIFY_FILTER_TYPE_DEVICEHANDLE)
    //   - handle CM_NOTIFY_ACTION_DEVICEQUERYREMOVE -> close handleDriver since device removal is requested
    //   - handle CM_NOTIFY_ACTION_DEVICEQUERYREMOVEFAILED -> query removal failed

    m_handleDriver = std::move(handleDriver);
    m_handleDriverNotifications = std::move(handleDriverNotifications);
    m_notificationThread = std::jthread([this] {
        HandleNotifications();
    });
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

    // Determine the amount of memory required for the UWB_DEVICE_CAPABILITIES from the driver.
    DWORD bytesRequired = 0;
    BOOL ioResult = DeviceIoControl(m_handleDriver.get(), IOCTL_UWB_GET_DEVICE_CAPABILITIES, nullptr, 0, nullptr, 0, &bytesRequired, nullptr);
    if (!LOG_IF_WIN32_BOOL_FALSE(ioResult)) {
        // TODO: need to do something different here
        HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
        PLOG_ERROR << "error when sending IOCTL_UWB_GET_DEVICE_CAPABILITIES, hr=" << std::showbase << std::hex << hr;
        return resultFuture;
    }

    // Allocate memory for the UWB_DEVICE_CAPABILITIES structure, and pass this to the driver request.
    DWORD uwbCapabilitiesSize = bytesRequired;
    auto uwbDeviceCapabilitiesBuffer = std::make_unique<uint8_t[]>(uwbCapabilitiesSize);
    ioResult = DeviceIoControl(m_handleDriver.get(), IOCTL_UWB_GET_DEVICE_CAPABILITIES, nullptr, 0, uwbDeviceCapabilitiesBuffer.get(), uwbCapabilitiesSize, &bytesRequired, nullptr);
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

void
UwbDeviceConnector::HandleNotifications()
{
    for (;;) {
        // TODO: check exit condition

        // Determine the amount of memory required for the UWB_NOTIFICATION_DATA from the driver.
        DWORD bytesRequired = 0;
        BOOL ioResult = DeviceIoControl(m_handleDriverNotifications.get(), IOCTL_UWB_NOTIFICATION, nullptr, 0, nullptr, 0, &bytesRequired, nullptr);
        if (!LOG_IF_WIN32_BOOL_FALSE(ioResult)) {
            HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
            PLOG_ERROR << "error determining output buffer size for UWB notification, hr=" << std::showbase << std::hex << hr;
            continue;
        }

        // Allocate memory for the UWB_NOTIFICATION_DATA structure, and pass this to the driver request.
        DWORD uwbNotificationDataSize = bytesRequired;
        std::vector<uint8_t> uwbNotificationDataBuffer(uwbNotificationDataSize);
        ioResult = DeviceIoControl(m_handleDriverNotifications.get(), IOCTL_UWB_NOTIFICATION, nullptr, 0, std::data(uwbNotificationDataBuffer), uwbNotificationDataSize, &bytesRequired, nullptr);
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

        // TODO: push this outside the class somewhere. This is where the pub/sub model comes into play.
        // OLD code below:
        // Invoke base class notification handler which takes care of threading.
        // ::UwbDevice::OnUwbNotification(std::move(uwbNotificationData));
    }
}
