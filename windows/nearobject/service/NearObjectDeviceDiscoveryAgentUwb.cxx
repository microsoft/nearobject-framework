
#include <nearobject/service/NearObjectDeviceUwb.hxx>
#include "NearObjectDeviceDiscoveryAgentUwb.hxx"
#include "UwbDevice.hxx"

using namespace windows::nearobject::service;

void
NearObjectDeviceDiscoveryAgentUwb::SignalDiscoveryEvent(::nearobject::service::NearObjectDevicePresence presence, std::unique_ptr<windows::devices::UwbDevice> uwbDevice) const noexcept
{
    static constexpr auto deviceIdFallback = 0x1234;

    // TODO: determine how devices will be assigned unique identifiers
    auto nearObjectDevice = std::make_shared<::nearobject::service::NearObjectDeviceUwb>(deviceIdFallback, std::move(uwbDevice));
    DevicePresenceChanged(presence, std::move(nearObjectDevice));
}
