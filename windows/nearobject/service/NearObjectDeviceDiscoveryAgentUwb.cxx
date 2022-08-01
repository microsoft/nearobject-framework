
#include "NearObjectDeviceDiscoveryAgentUwb.hxx"
#include "UwbDevice.hxx"
#include <nearobject/service/NearObjectDeviceUwb.hxx>

using namespace windows::nearobject::service;
using ::nearobject::service::NearObjectDevicePresence;

std::shared_ptr<::nearobject::service::NearObjectDeviceUwb>
NearObjectDeviceDiscoveryAgentUwb::SignalDeviceAdded(std::unique_ptr<windows::devices::UwbDevice> uwbDevice) const noexcept
{
    static constexpr auto deviceIdFallback = 0x1234;

    // TODO: determine how devices will be assigned unique identifiers
    auto nearObjectDeviceUwb = std::make_shared<::nearobject::service::NearObjectDeviceUwb>(deviceIdFallback, std::move(uwbDevice));
    DevicePresenceChanged(NearObjectDevicePresence::Arrived, nearObjectDeviceUwb);
    return nearObjectDeviceUwb;
}

void
NearObjectDeviceDiscoveryAgentUwb::SignalDeviceRemoved(std::shared_ptr<::nearobject::service::NearObjectDeviceUwb> nearObjectDeviceUwb) const noexcept
{
    DevicePresenceChanged(NearObjectDevicePresence::Departed, std::move(nearObjectDeviceUwb));
}
