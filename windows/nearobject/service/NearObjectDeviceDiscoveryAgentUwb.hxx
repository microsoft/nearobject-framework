
#ifndef NEAR_OBJECT_DEVICE_DISCOVERY_AGENT_UWB
#define NEAR_OBJECT_DEVICE_DISCOVERY_AGENT_UWB

#include <future>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

#include <windows.h>
#include <cfgmgr32.h>
#include <devpkey.h>
#include <wil/resource.h>

#include <nearobject/service/NearObjectDeviceDiscoveryAgent.hxx>
#include <WindowsDeviceResource.hxx>

namespace nearobject
{
namespace service
{
class NearObjectDevice;
class NearObjectDeviceUwb;
} // namespace service
} // namespace nearobject

namespace windows
{
namespace devices
{
class UwbDevice;
}
namespace nearobject
{
namespace service
{
class NearObjectDeviceDiscoveryAgentUwb :
    public ::nearobject::service::NearObjectDeviceDiscoveryAgent
{
protected:
    std::future<std::vector<std::shared_ptr<::nearobject::service::NearObjectDevice>>>
    ProbeAsyncImpl() override;

private:
    /**
     * @brief Register for notifications of UWB device driver events.
     */
    void
    RegisterForUwbDeviceClassNotifications();

    /**
     * @brief Bound callback function for when a UWB class driver event occurs. 
     * 
     * @param hcmNotificationHandle 
     * @param action 
     * @param eventData 
     * @param eventDataSize 
     */
    void
    OnDeviceInterfaceNotification(HCMNOTIFICATION hcmNotificationHandle, CM_NOTIFY_ACTION action, CM_NOTIFY_EVENT_DATA *eventData, DWORD eventDataSize);

    /**
     * @brief Unbound callback function for when a UWB class driver event occurs. 
     * 
     * @param hcmNotificationHandle 
     * @param context 
     * @param action 
     * @param eventData 
     * @param eventDataSize 
     * @return DWORD 
     */
    static DWORD CALLBACK
    OnDeviceInterfaceNotificationCallback(HCMNOTIFICATION hcmNotificationHandle, void *context, CM_NOTIFY_ACTION action, CM_NOTIFY_EVENT_DATA *eventData, DWORD eventDataSize);

    /**
     * @brief Create (if necessary) and add NearObjectDeviceUwb wrapper instance
     * to the device cache.
     * 
     * @param deviceName The device driver name to add/create.
     * @return std::shared_ptr<::nearobject::service::NearObjectDeviceUwb> 
     */
    std::shared_ptr<::nearobject::service::NearObjectDeviceUwb>
    AddCachedUwbNearObjectDevice(const std::wstring &deviceName);

    /**
     * @brief Remove and return a NearObjectDeviceUwb device driver instance
     * from the device cache.
     * 
     * @param deviceName The device driver name to remove.
     * @return std::shared_ptr<::nearobject::service::NearObjectDeviceUwb> 
     */
    std::shared_ptr<::nearobject::service::NearObjectDeviceUwb>
    ExtractCachedNearObjectDevice(const std::wstring &deviceName);

private:
    unique_hcmnotification m_uwbHcmNotificationHandle;

    std::mutex m_nearObjectDeviceCacheGate;
    std::unordered_map<std::wstring, std::weak_ptr<::nearobject::service::NearObjectDeviceUwb>> m_nearObjectDeviceCache;
};
} // namespace service
} // namespace nearobject
} // namespace windows

#endif // NEAR_OBJECT_DEVICE_DISCOVERY_AGENT_UWB
