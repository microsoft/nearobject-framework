
#include <tuple>

#include <uwb/UwbPeer.hxx>

using namespace uwb;

UwbPeer::UwbPeer(UwbMacAddress address) :
    m_address(std::move(address))
{}

UwbPeer::UwbPeer(const UwbPeer& other) :
    m_address(other.m_address),
    m_spatialProperties(other.m_spatialProperties)
{}

UwbPeer::UwbPeer(UwbPeer&& other) noexcept
{
    std::swap(m_address, other.m_address);
    std::swap(m_spatialProperties, other.m_spatialProperties);
}

UwbPeer& 
UwbPeer::operator=(const UwbPeer& other)
{
    if (this == &other) {
        return *this;
    }

    m_address = other.m_address;
    m_spatialProperties = other.m_spatialProperties;
    return *this;
}

UwbMacAddress 
UwbPeer::GetAddress() const noexcept
{
    return m_address;
}

UwbPeerSpatialProperties
UwbPeer::GetSpatialProperties() const noexcept
{
    const auto lock = std::scoped_lock{ m_spatialPropertiesGate };
    return m_spatialProperties;
}

bool
uwb::operator==(const UwbPeer& lhs, const UwbPeer& rhs) noexcept
{
    return lhs.GetAddress() == rhs.GetAddress();
}

bool
uwb::operator!=(const UwbPeer& lhs, const UwbPeer& rhs) noexcept
{
    return !(lhs == rhs);
}

bool
uwb::operator==(const UwbPeerSpatialProperties& lhs, const UwbPeerSpatialProperties& rhs) noexcept
{
    return std::tie(lhs.Distance, lhs.AngleAzimuth, lhs.AngleElevation, lhs.Elevation)
        == std::tie(rhs.Distance, rhs.AngleAzimuth, rhs.AngleElevation, rhs.Elevation);
}

bool
uwb::operator!=(const UwbPeerSpatialProperties& lhs, const UwbPeerSpatialProperties& rhs) noexcept
{
    return !(lhs == rhs);
}
