
#include <nearobject/service/NearObjectDeviceControllerUwb.hxx>

#include <nearobject/NearObjectSessionEventCallbacks.hxx>

using namespace nearobject::service;

namespace
{
static constexpr auto deviceIdFallback = 0x1234;
} // namespace detail

NearObjectDeviceControllerUwb::NearObjectDeviceControllerUwb(std::unique_ptr<uwb::UwbDevice> uwbDevice) :
    NearObjectDeviceController(deviceIdFallback),
    m_uwbDevice(std::move(uwbDevice))
{
    // TODO: obtain device GUID from uwbDevice and use that in place of deviceIdFallback above
}

NearObjectDeviceController::StartSessionResult
NearObjectDeviceControllerUwb::StartSessionImpl(const NearObjectProfile& /* profile */, std::weak_ptr<NearObjectSessionEventCallbacks> /* eventCallbacks */)
{
    return { std::nullopt };
}
