
#include <linux/uwb/UwbDevice.hxx>

using namespace linux::devices;
using namespace uwb::protocol::fira;

std::shared_ptr<uwb::UwbSession>
UwbDevice::CreateSessionImpl(uint32_t sessionId, std::weak_ptr<uwb::UwbSessionEventCallbacks> /* callbacks */)
{
    // TODO
    return nullptr;
}

UwbCapability
UwbDevice::GetCapabilitiesImpl()
{
    // TODO
    return {};
}

UwbDeviceInformation
GetDeviceInformationImpl()
{
    // TODO
    return {};
}

void
ResetImpl()
{
}

bool
UwbDevice::IsEqual(const uwb::UwbDevice& other) const noexcept
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
    const auto& rhs = static_cast<const linux::devices::UwbDevice&>(other);

    // TODO: implement this properly
    return (this == &rhs);
}
