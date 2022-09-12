
#ifndef NEAR_OBJECT_DEVICE_DISCOVERY_AGENT_UWB
#define NEAR_OBJECT_DEVICE_DISCOVERY_AGENT_UWB

#include <future>
#include <memory>
#include <vector>

#include <nearobject/service/NearObjectDeviceControllerDiscoveryAgent.hxx>

namespace nearobject
{
namespace service
{
class NearObjectDeviceController;
class NearObjectDeviceUwb;
} // namespace service
} // namespace nearobject

namespace linux
{
namespace nearobject
{
namespace service
{
class NearObjectDeviceDiscoveryAgentUwb :
    public ::nearobject::service::NearObjectDeviceControllerDiscoveryAgent
{
protected:
    std::future<std::vector<std::shared_ptr<::nearobject::service::NearObjectDeviceController>>>
    ProbeAsyncImpl() override;

private:
    std::vector<std::shared_ptr<::nearobject::service::NearObjectDeviceController>>
    Probe();
};
} // namespace service
} // namespace nearobject
} // namespace windows

#endif // NEAR_OBJECT_DEVICE_DISCOVERY_AGENT_UWB
