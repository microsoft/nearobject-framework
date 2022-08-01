
#ifndef NEAR_OBJECT_DEVICE_DISCOVERY_AGENT_UWB
#define NEAR_OBJECT_DEVICE_DISCOVERY_AGENT_UWB

#include <memory>

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
    void
    SignalDiscoveryEvent(::nearobject::service::NearObjectDevicePresence presence, std::unique_ptr<windows::devices::UwbDevice> uwbDevice) const noexcept;
};
} // namespace service
} // namespace nearobject
} // namespace windows

#endif // NEAR_OBJECT_DEVICE_DISCOVERY_AGENT_UWB