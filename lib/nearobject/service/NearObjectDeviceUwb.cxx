
#include "NearObjectDeviceUwb.hxx"

#include <nearobject/NearObjectSessionEventCallbacks.hxx>

using namespace nearobject::service;

namespace
{
static constexpr auto deviceIdFallback = 0x1234;
} // namespace detail

NearObjectDeviceUwb::NearObjectDeviceUwb(std::unique_ptr<uwb::UwbDevice> uwbDevice) :
    NearObjectDevice(deviceIdFallback),
    m_uwbDevice(std::move(uwbDevice))
{
    // TODO: obtain device GUID from uwbDevice and use that in place of deviceIdFallback above
}

NearObjectDevice::StartSessionResult
NearObjectDeviceUwb::StartSessionImpl(const NearObjectProfile& /* profile */, std::weak_ptr<NearObjectSessionEventCallbacks> /* eventCallbacks */)
{
    return { std::nullopt };
}
