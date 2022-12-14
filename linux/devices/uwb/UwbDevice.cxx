
#include <linux/uwb/UwbDevice.hxx>

using namespace linux::devices;

std::unique_ptr<uwb::UwbSession>
UwbDevice::CreateSession(std::weak_ptr<uwb::UwbSessionEventCallbacks> /* callbacks */)
{
    // TODO
    return nullptr;
}

uwb::protocol::fira::UwbCapability
UwbDevice::GetCapabilities() const
{
    // TODO
    return {};
}

bool
UwbDevice::IsEqual(const uwb::UwbDevice& other) const noexcept
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
    const auto& rhs = static_cast<const linux::devices::UwbDevice&>(other);

    // TODO: implement this properly
    return (this == &rhs);
}
