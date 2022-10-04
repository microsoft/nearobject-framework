
#include <linux/nearobject/service/NearObjectDeviceDiscoveryAgentUwb.hxx>
#include <nearobject/service/NearObjectDeviceControllerUwb.hxx>

using namespace linux::nearobject::service;
using ::nearobject::service::NearObjectDeviceController;

std::vector<std::shared_ptr<::nearobject::service::NearObjectDeviceController>>
NearObjectDeviceDiscoveryAgentUwb::Probe()
{
    // TODO: implement me
    std::vector<std::shared_ptr<::nearobject::service::NearObjectDeviceController>> nearObjectDevices;
    return nearObjectDevices;
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
