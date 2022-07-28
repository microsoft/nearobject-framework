
#include "NearObjectDeviceUwb.hxx"

#include <nearobject/NearObjectSessionEventCallbacks.hxx>

using namespace nearobject::service;

NearObjectDeviceUwb::NearObjectDeviceUwb(std::unique_ptr<uwb::UwbDevice> uwbDevice) :
    m_uwbDevice(std::move(uwbDevice))
{}

NearObjectDevice::StartSessionResult
NearObjectDeviceUwb::StartSessionImpl(const NearObjectProfile& /* profile */, std::weak_ptr<NearObjectSessionEventCallbacks> /* eventCallbacks */)
{
    return { std::nullopt };
}
