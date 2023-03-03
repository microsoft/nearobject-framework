
#include <future>
#include <ios>
#include <stdexcept>

#include <windows/devices/uwb/UwbCxAdapterDdiLrp.hxx>
#include <windows/devices/uwb/UwbCxDdiLrp.hxx>
#include <windows/devices/uwb/UwbDevice.hxx>
#include <windows/devices/uwb/UwbSession.hxx>

#include <plog/Log.h>
#include <wil/result.h>

using namespace windows::devices::uwb;
using namespace ::uwb::protocol::fira;

/**
 * @brief Namespace alias to reduce typing but preserve clarity regarding DDI
 * conversion.
 */
namespace UwbCxDdi = windows::devices::uwb::ddi::lrp;

UwbDevice::UwbDevice(std::string deviceName) :
    m_deviceName(std::move(deviceName))
{}

wil::shared_hfile
UwbDevice::DriverHandle() noexcept
{
    return m_handleDriver;
}

wil::shared_hfile
UwbDevice::DriverHandleNotifications() noexcept
{
    return m_handleDriverNotifications;
}

const std::string&
UwbDevice::DeviceName() const noexcept
{
    return m_deviceName;
}

void
UwbDevice::Initialize()
{
    m_uwbDeviceConnector = std::make_shared<UwbDeviceConnector>(m_deviceName);
    m_uwbDeviceConnector->NotificationListenerStart([this](auto&& uwbNotificationData)
    {
        // Invoke base class notification handler which takes care of threading.
        ::UwbDevice::OnUwbNotification(std::move(uwbNotificationData));
    });

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

void
UwbDevice::HandleNotifications()
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

        // Invoke base class notification handler which takes care of threading.
        ::UwbDevice::OnUwbNotification(std::move(uwbNotificationData));
    }
}

std::shared_ptr<uwb::UwbSession>
UwbDevice::CreateSessionImpl(std::weak_ptr<::uwb::UwbSessionEventCallbacks> callbacks)
{
    return CreateSessionImpl<UwbSession>(std::move(callbacks));
}

UwbCapability
UwbDevice::GetCapabilitiesImpl()
{
    auto resultFuture = m_uwbDeviceConnector->GetCapabilities();
    if (!resultFuture.valid()) {
        // TODO: need to do something different than just return a default-constructed object here
        PLOG_ERROR << "failed to obtain capabilities from driver";
        return {};
    }

    try {
        auto [uwbStatus, uwbCapability] = resultFuture.get();
        if (!IsUwbStatusOk(uwbStatus)) {
            PLOG_ERROR << "uwb device reported an error obtaining uwb capabilities, status =" << ::ToString(uwbStatus);
            return {};
        }
        return std::move(uwbCapability);
    } catch (std::exception& e) {
        PLOG_ERROR << "caught exception obtaining uwb capabilities (" << e.what() << ")";
        return {};
    }
}

bool
UwbDevice::IsEqual(const ::uwb::UwbDevice& other) const noexcept
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
    const auto& rhs = static_cast<const windows::devices::uwb::UwbDevice&>(other);
    return (this->DeviceName() == rhs.DeviceName());
}
