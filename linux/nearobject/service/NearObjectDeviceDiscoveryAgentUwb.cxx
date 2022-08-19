
#include "NearObjectDeviceDiscoveryAgentUwb.hxx"

#include <nearobject/service/NearObjectDeviceUwb.hxx>

using namespace linux::nearobject::service;
using ::nearobject::service::NearObjectDevice;
using ::nearobject::service::NearObjectDevicePresence;
using ::nearobject::service::NearObjectDeviceUwb;

std::vector<std::shared_ptr<::nearobject::service::NearObjectDevice>>
NearObjectDeviceDiscoveryAgentUwb::Probe()
{
    // TODO: implement me
    std::vector<std::shared_ptr<::nearobject::service::NearObjectDevice>> nearObjectDevices;
    return nearObjectDevices;
}

std::future<std::vector<std::shared_ptr<NearObjectDevice>>>
NearObjectDeviceDiscoveryAgentUwb::ProbeAsyncImpl()
{
    // TODO: this instance must be kept alive/valid until the async operation is
    // complete since it accesses members.
    return std::async(std::launch::async, [&](){
        return Probe();
    });
}
