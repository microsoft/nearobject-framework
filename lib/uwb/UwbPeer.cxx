
#include <cmath>
#include <tuple>

#include <notstd/tostring.hxx>
#include <sstream>
#include <string>
#include <uwb/UwbPeer.hxx>

using namespace uwb;
using namespace strings::ostream_operators;

std::string
UwbPeerSpatialProperties::ToString() const
{
    static const std::initializer_list<std::tuple<std::string, std::optional<double>>> name2value{
        { "Distance", Distance },
        { "AngleAzimuth", AngleAzimuth },
        { "AngleElevation", AngleElevation },
        { "Elevation", Elevation }
    };

    std::ostringstream ss;

    for (const auto& [name, value] : name2value) {
        ss << name << ": " << std::to_string(value.value_or(0)) << ", ";
    }

    // Remove the trailing ", "
    auto str = ss.str();
    str.resize(std::size(str) - 2);

    return str;
}

UwbPeer::UwbPeer(UwbMacAddress address) :
    m_address(std::move(address))
{}

UwbPeer::UwbPeer(const uwb::protocol::fira::UwbRangingMeasurement& data) :
    m_address{ data.PeerMacAddress },
    m_spatialProperties{
        .Distance{ data.Distance },
        .AngleAzimuth{ uwb::protocol::fira::ConvertQ97FormatToIEEE(data.AoAAzimuth.Result) },
        .AngleElevation{ uwb::protocol::fira::ConvertQ97FormatToIEEE(data.AoAElevation.Result) },
        .Elevation{ uwb::protocol::fira::ConvertQ97FormatToIEEE(data.AoaDestinationElevation.Result) },

        .AngleAzimuthFom{ data.AoAAzimuth.FigureOfMerit },
        .AngleElevationFom{ data.AoAElevation.FigureOfMerit },
        .ElevationFom{ data.AoaDestinationElevation.FigureOfMerit }
    }
{
}

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
    std::ostringstream ss;
    ss << "[" << m_address << "] " << m_spatialProperties;
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
