
#include <string>

#include <nearobject/service/NearObjectDeviceUwb.hxx>
#include <windows/uwb/UwbDevice.hxx>
#include <windows/uwb/UwbDeviceDriver.hxx>
#include <windows/devices/DeviceEnumerator.hxx>
#include <windows/nearobject/service/NearObjectDeviceDiscoveryAgentUwb.hxx>

using namespace windows::nearobject::service;
using ::nearobject::service::NearObjectDeviceController;
using ::nearobject::service::NearObjectDevicePresence;
using ::nearobject::service::NearObjectDeviceUwb;

namespace
{
std::shared_ptr<NearObjectDeviceUwb> CreateNearObjectUwbDevice(std::wstring deviceName)
{
    auto uwbDevice = std::make_unique<windows::devices::UwbDevice>(std::move(deviceName));
    return std::make_shared<NearObjectDeviceUwb>(std::move(uwbDevice));
}
}

std::shared_ptr<NearObjectDeviceUwb>
NearObjectDeviceDiscoveryAgentUwb::AddCachedUwbNearObjectDevice(const std::wstring &deviceName)
{
    // Note: the exclusive lock must be held for the duration, even during the
    // cache lookup parts to prevent races with concurrent notifications adding
    // entries to the cache.
    const auto nearObjectDeviceLock = std::scoped_lock{ m_nearObjectDeviceCacheGate };

    // Attempt to find a cached device instance.
    const auto nearObjectDeviceCacheNode = m_nearObjectDeviceCache.find(deviceName);
    if (nearObjectDeviceCacheNode != std::cend(m_nearObjectDeviceCache)) {
        // Cached instance found; attempt to promote it to shared_ptr.
        auto nearObjectDeviceWeak = nearObjectDeviceCacheNode->second;
        auto nearObjectDevice = nearObjectDeviceWeak.lock();
        if (nearObjectDevice != nullptr) {
            return nearObjectDevice;
        }
        // Cached instance is no longer valid. Remove it, then fall-through to
        // below to create and add a new instance.
        m_nearObjectDeviceCache.erase(nearObjectDeviceCacheNode);
    }

    // Create a new instance and add it to the cache.
    auto nearObjectDevice = CreateNearObjectUwbDevice(deviceName);
    m_nearObjectDeviceCache[deviceName] = nearObjectDevice;

    return nearObjectDevice;
}

std::shared_ptr<NearObjectDeviceUwb>
NearObjectDeviceDiscoveryAgentUwb::ExtractCachedNearObjectDevice(const std::wstring &deviceName)
{
    auto nearObjectDeviceLock = std::scoped_lock{ m_nearObjectDeviceCacheGate };
    auto nearObjectExtractResult = m_nearObjectDeviceCache.extract(deviceName);
    return nearObjectExtractResult.empty()
        ? nullptr
        : nearObjectExtractResult.mapped().lock();
}

void
NearObjectDeviceDiscoveryAgentUwb::RegisterForUwbDeviceClassNotifications()
{
    CM_NOTIFY_FILTER interfaceFilter{};
    interfaceFilter.cbSize = sizeof interfaceFilter;
    interfaceFilter.FilterType = CM_NOTIFY_FILTER_TYPE_DEVICEINTERFACE;
    interfaceFilter.u.DeviceInterface.ClassGuid = windows::devices::uwb::InterfaceClassUwb;

    auto notifyRegistrationResult = CM_Register_Notification(&interfaceFilter, this, OnDeviceInterfaceNotificationCallback, &m_uwbHcmNotificationHandle);
    if (notifyRegistrationResult != CR_SUCCESS) {
        // TODO: log? throw?
    }
}

void
NearObjectDeviceDiscoveryAgentUwb::UnregisterForUwbDeviceClassNotifications()
{
    m_uwbHcmNotificationHandle.reset();
    // TODO: anything else needed here?
}

void
NearObjectDeviceDiscoveryAgentUwb::OnDeviceInterfaceNotification(HCMNOTIFICATION hcmNotificationHandle, CM_NOTIFY_ACTION action, CM_NOTIFY_EVENT_DATA *eventData, DWORD eventDataSize)
{
    std::wstring deviceName;

    switch (action) {
        case CM_NOTIFY_ACTION_DEVICEINTERFACEARRIVAL: {
            deviceName = eventData->u.DeviceInterface.SymbolicLink;
            auto nearObjectDeviceUwb = AddCachedUwbNearObjectDevice(deviceName);
            DevicePresenceChanged(NearObjectDevicePresence::Arrived, std::move(nearObjectDeviceUwb));
            break;
        }
        case CM_NOTIFY_ACTION_DEVICEINTERFACEREMOVAL: {
            deviceName = eventData->u.DeviceInterface.SymbolicLink;
            auto nearObjectDeviceUwb = ExtractCachedNearObjectDevice(deviceName);
            if (nearObjectDeviceUwb) {
                DevicePresenceChanged(NearObjectDevicePresence::Departed, std::move(nearObjectDeviceUwb));
            }
            break;
        }
        default:
            break;
    }
}

/* static */
DWORD CALLBACK
NearObjectDeviceDiscoveryAgentUwb::OnDeviceInterfaceNotificationCallback(HCMNOTIFICATION hcmNotificationHandle, void *context, CM_NOTIFY_ACTION action, CM_NOTIFY_EVENT_DATA *eventData, DWORD eventDataSize)
{
    auto self = static_cast<NearObjectDeviceDiscoveryAgentUwb *>(context);
    self->OnDeviceInterfaceNotification(hcmNotificationHandle, action, eventData, eventDataSize);
    return S_OK;
}

/* static */
std::vector<std::shared_ptr<::nearobject::service::NearObjectDeviceController>>
NearObjectDeviceDiscoveryAgentUwb::Probe()
{
    const auto deviceInterfaceNames = windows::devices::DeviceEnumerator::GetDeviceInterfaceClassInstanceNames(windows::devices::uwb::InterfaceClassUwb);

    std::vector<std::shared_ptr<::nearobject::service::NearObjectDeviceController>> nearObjectDevices;
    for (const auto& deviceInterfaceName : deviceInterfaceNames) {
        auto nearObjectDevice = AddCachedUwbNearObjectDevice(deviceInterfaceName);
        if (nearObjectDevice) {
            nearObjectDevices.push_back(std::move(nearObjectDevice));
        }
    }

    return nearObjectDevices;
}

void
NearObjectDeviceDiscoveryAgentUwb::StartImpl()
{
    RegisterForUwbDeviceClassNotifications();
}

void
NearObjectDeviceDiscoveryAgentUwb::StopImpl()
{
    UnregisterForUwbDeviceClassNotifications();
}

std::future<std::vector<std::shared_ptr<NearObjectDeviceController>>>
NearObjectDeviceDiscoveryAgentUwb::ProbeAsyncImpl()
{
    // TODO: this instance must be kept alive/valid until the async operation is
    // complete since it accesses members.
    return std::async(std::launch::async, [&](){
        return Probe(); 
    });
}
