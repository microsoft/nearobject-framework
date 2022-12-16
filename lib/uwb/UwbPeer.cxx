
#include <tuple>

#include <sstream>
#include <string>
#include <uwb/UwbPeer.hxx>
using namespace uwb;

std::string
UwbPeerSpatialProperties::ToString() const
{
    std::initializer_list<std::tuple<std::string, std::optional<double>>> name2value{ { "Distance", Distance }, 
                                                                                    { "AngleAzimuth", AngleAzimuth }, 
                                                                                    { "AngleElevation", AngleElevation }, 
                                                                                    { "Elevation", Elevation } };

    std::stringstream ss;

    for (const auto& [name, value] : name2value) {
        if (value.has_value()) {
            ss << name << ": " << std::to_string(*value) << std::endl;
        }
    }

    return ss.str();
}

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

std::string
UwbPeer::ToString() const
{
    std::stringstream ss;
    ss << "{" << std::endl;
    ss << "mac: " << m_address.ToString() << std::endl;
    ss << m_spatialProperties.ToString() << std::endl;
    ss << "}" << std::endl;
    return ss.str();
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
