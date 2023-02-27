
#include <filesystem>

#include <windows/devices/DevicePresenceMonitor.hxx>

#include <plog/Log.h>

using namespace windows::devices;

DevicePresenceMonitor::DevicePresenceMonitor(const GUID &deviceGuid, std::function<void(const GUID &deviceGuid, DevicePresenceEvent presenceEvent, std::string deviceName)> callback) noexcept :
    DevicePresenceMonitor(std::vector<GUID>{ deviceGuid }, std::move(callback))
{}

DevicePresenceMonitor::DevicePresenceMonitor(std::vector<GUID> deviceGuids, std::function<void(const GUID &deviceGuid, DevicePresenceEvent presenceEvent, std::string deviceName)> callback) noexcept :
    m_callback(std::move(callback))
{
    for (auto &deviceGuid : deviceGuids) {
        m_deviceGuids.insert({ std::move(deviceGuid), unique_hcmnotification{} });
    }
}

void
DevicePresenceMonitor::RegisterForDeviceClassNotifications()
{
    for (auto &[deviceGuid, hcmNotificationHandle] : m_deviceGuids) {
        CM_NOTIFY_FILTER interfaceFilter = {};
        interfaceFilter.cbSize = sizeof interfaceFilter;
        interfaceFilter.FilterType = CM_NOTIFY_FILTER_TYPE_DEVICEINTERFACE;
        interfaceFilter.u.DeviceInterface.ClassGuid = deviceGuid;

        auto notifyRegistrationResult = CM_Register_Notification(&interfaceFilter, this, OnDeviceInterfaceNotificationCallback, &hcmNotificationHandle);
        if (notifyRegistrationResult != CR_SUCCESS) {
            PLOG_ERROR << "failed to register notification callback for device guid " << notstd::GuidToString<std::string>(deviceGuid);
            throw DevicePresenceMonitor::StartException{ deviceGuid, notifyRegistrationResult };
        }
    }
}

void
DevicePresenceMonitor::UnregisterForDeviceClassNotifications()
{
    for (auto &[_, hcmNotificationHandle] : m_deviceGuids) {
        hcmNotificationHandle.reset();
    }
}

void
DevicePresenceMonitor::OnDeviceInterfaceNotification(HCMNOTIFICATION hcmNotificationHandle, CM_NOTIFY_ACTION action, CM_NOTIFY_EVENT_DATA *eventData, DWORD eventDataSize)
{
    std::string deviceName;
    std::filesystem::path devicePath;

    // N.B.: The device interface paths provided are UTF-16 wchar_t-based
    // strings (eg. std::wstring'ish), but this library has standardized on
    // std::string with any valid encoding (eg. UTF-8), so a conversion is
    // necessary. The std::filesystem::path interface allows converting the
    // internal path to either generic (eg. POSIX-style) or native formats, and
    // overloads exist for all of the std string types. Consequently, we use the
    // native version below to convert from std::wstring to std::string. It is
    // assumed that the path names provided by this API (from the win32
    // namespace) contain only ASCII characters, and so, the conversion should
    // be safe.

    switch (action) {
    case CM_NOTIFY_ACTION_DEVICEINTERFACEARRIVAL: {
        devicePath = std::filesystem::path(std::wstring(eventData->u.DeviceInterface.SymbolicLink));
        deviceName = devicePath.string();
        m_callback(eventData->u.DeviceInterface.ClassGuid, DevicePresenceEvent::Arrived, std::move(deviceName));
        break;
    }
    case CM_NOTIFY_ACTION_DEVICEINTERFACEREMOVAL: {
        devicePath = std::filesystem::path(std::wstring(eventData->u.DeviceInterface.SymbolicLink));
        deviceName = devicePath.string();
        m_callback(eventData->u.DeviceInterface.ClassGuid, DevicePresenceEvent::Departed, std::move(deviceName));
        break;
    }
    default:
        break;
    }
}

/* static */
DWORD CALLBACK
DevicePresenceMonitor::OnDeviceInterfaceNotificationCallback(HCMNOTIFICATION hcmNotificationHandle, void *context, CM_NOTIFY_ACTION action, CM_NOTIFY_EVENT_DATA *eventData, DWORD eventDataSize)
{
    auto self = static_cast<DevicePresenceMonitor *>(context);
    self->OnDeviceInterfaceNotification(hcmNotificationHandle, action, eventData, eventDataSize);
    return S_OK;
}

void
DevicePresenceMonitor::Start()
{
    RegisterForDeviceClassNotifications();
}

void
DevicePresenceMonitor::Stop()
{
    UnregisterForDeviceClassNotifications();
}
