
#ifndef NEAR_OBJECT_DEVICE_DISCOVERY_AGENT_UWB
#define NEAR_OBJECT_DEVICE_DISCOVERY_AGENT_UWB

#include <memory>

#include <nearobject/service/NearObjectDeviceUwb.hxx>
#include <nearobject/service/NearObjectDeviceDiscoveryAgent.hxx>

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
private:
    std::shared_ptr<::nearobject::service::NearObjectDeviceUwb>
    SignalDeviceAdded(std::unique_ptr<windows::devices::UwbDevice> uwbDevice) const noexcept;

    void
    SignalDeviceRemoved(std::shared_ptr<::nearobject::service::NearObjectDeviceUwb> nearObjectDeviceUwb) const noexcept;
};
} // namespace service
} // namespace nearobject
} // namespace windows

#endif // NEAR_OBJECT_DEVICE_DISCOVERY_AGENT_UWB