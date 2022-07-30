
#include "NearObjectDeviceUwb.hxx"

#include <nearobject/NearObjectSessionEventCallbacks.hxx>

using namespace nearobject::service;

NearObjectDeviceUwb::NearObjectDeviceUwb(uint64_t deviceId, std::unique_ptr<uwb::UwbDevice> uwbDevice) :
    NearObjectDevice(deviceId),
    m_uwbDevice(std::move(uwbDevice))
{}

NearObjectDevice::StartSessionResult
NearObjectDeviceUwb::StartSessionImpl(const NearObjectProfile& /* profile */, std::weak_ptr<NearObjectSessionEventCallbacks> /* eventCallbacks */)
{
    return { std::nullopt };
}
