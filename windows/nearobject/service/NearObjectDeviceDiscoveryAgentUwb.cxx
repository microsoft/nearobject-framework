
#include <string>

#include "NearObjectDeviceDiscoveryAgentUwb.hxx"

#include <UwbDevice.hxx>
#include <UwbDeviceDriver.hxx>
#include <nearobject/service/NearObjectDeviceUwb.hxx>

using namespace windows::nearobject::service;
using ::nearobject::service::NearObjectDevice;
using ::nearobject::service::NearObjectDevicePresence;
using ::nearobject::service::NearObjectDeviceUwb;

std::shared_ptr<NearObjectDeviceUwb>
NearObjectDeviceDiscoveryAgentUwb::AddCachedUwbNearObjectDevice(const std::wstring &deviceName)
{
    // Note: the exclusive lock must be held for the duration, even during the
    // cache lookup parts to prevent races with concurrent notifications adding
    // entries to the cache.
    const auto nearObjectDeviceLockExclusive = std::scoped_lock{ m_nearObjectDeviceCacheGate };

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
    auto uwbDevice = std::make_unique<windows::devices::UwbDevice>(deviceName);
    auto nearObjectDevice = std::make_shared<NearObjectDeviceUwb>(std::move(uwbDevice));
    m_nearObjectDeviceCache[deviceName] = nearObjectDevice;

    return nearObjectDevice;
}

std::shared_ptr<NearObjectDeviceUwb>
NearObjectDeviceDiscoveryAgentUwb::ExtractCachedNearObjectDevice(const std::wstring &deviceName)
{
    auto nearObjectDeviceLockExclusive = std::unique_lock{ m_nearObjectDeviceCacheGate };
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

std::future<std::vector<std::weak_ptr<NearObjectDevice>>>
NearObjectDeviceDiscoveryAgentUwb::ProbeAsyncImpl()
{
    std::promise<std::vector<std::weak_ptr<NearObjectDevice>>> probePromise{};
    // TODO: save promise somewhere
    // TODO: start async operation
    // TODO: call CM_Get_Device_InterfaceList.
    // TODO: call probePromise.set_value({/* results from CM_Get_Device_InterfaceList */})
    return probePromise.get_future();
}
