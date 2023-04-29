
#ifndef NEAR_OBJECT_DEVICE_DISCOVERY_AGENT_UWB
#define NEAR_OBJECT_DEVICE_DISCOVERY_AGENT_UWB

#include <future>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

#include <nearobject/service/NearObjectDeviceControllerDiscoveryAgent.hxx>
#include <windows/devices/DevicePresenceMonitor.hxx>
#include <windows/devices/DeviceResource.hxx>

namespace nearobject
{
namespace service
{
class NearObjectDeviceController;
class NearObjectDeviceControllerUwb;
} // namespace service
} // namespace nearobject

namespace windows
{
namespace devices::uwb
{
class UwbDevice;
} // namespace devices::uwb

namespace nearobject::service
{
class NearObjectDeviceDiscoveryAgentUwb :
    public ::nearobject::service::NearObjectDeviceControllerDiscoveryAgent
{
public:
    NearObjectDeviceDiscoveryAgentUwb();

protected:
    void
    StartImpl() override;

    void
    StopImpl() override;

    std::future<std::vector<std::shared_ptr<::nearobject::service::NearObjectDeviceController>>>
    ProbeAsyncImpl() override;

private:
    std::vector<std::shared_ptr<::nearobject::service::NearObjectDeviceController>>
    Probe();

    /**
     * @brief Create (if necessary) and add NearObjectDeviceControllerUwb wrapper instance
     * to the device cache.
     *
     * @param deviceName The device driver name to add/create.
     * @return std::shared_ptr<::nearobject::service::NearObjectDeviceControllerUwb>
     */
    std::shared_ptr<::nearobject::service::NearObjectDeviceControllerUwb>
    AddCachedUwbNearObjectDevice(const std::string &deviceName);

    /**
     * @brief Remove and return a NearObjectDeviceControllerUwb device driver instance
     * from the device cache.
     *
     * @param deviceName The device driver name to remove.
     * @return std::shared_ptr<::nearobject::service::NearObjectDeviceControllerUwb>
     */
    std::shared_ptr<::nearobject::service::NearObjectDeviceControllerUwb>
    ExtractCachedNearObjectDevice(const std::string &deviceName);

    /**
     * @brief Callback function invoked when device presence changes.
     *
     * @param deviceGuid The GUID of the device that changed presence.
     * @param presenceEvent The type of presence change that ocurred.
     * @param deviceName The device name (interface path) of the device.
     */
    void
    OnDevicePresenceChanged(const GUID &deviceGuid, windows::devices::DevicePresenceEvent presenceEvent, std::string deviceName);

private:
    std::mutex m_nearObjectDeviceCacheGate;
    std::unordered_map<std::string, std::weak_ptr<::nearobject::service::NearObjectDeviceControllerUwb>> m_nearObjectDeviceCache;

    // this member is actually in charge of handling the presence monitoring
    windows::devices::DevicePresenceMonitor m_devicePresenceMonitor;
};
} // namespace nearobject::service
} // namespace windows

#endif // NEAR_OBJECT_DEVICE_DISCOVERY_AGENT_UWB
