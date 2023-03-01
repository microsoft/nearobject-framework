
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
    std::initializer_list<std::tuple<std::string, std::optional<double>>> name2value{ { "Distance", Distance },
        { "AngleAzimuth", AngleAzimuth },
        { "AngleElevation", AngleElevation },
        { "Elevation", Elevation } };

    std::ostringstream ss;

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

/**
 * @brief Converts a Q9.7-formatted value to an IEEE 754 double precision floating point formatted value.
 *
 * Assuming the Arm definition of Qm.n formatting, the most significant bit is the sign, the next
 * (m-1) bits are an integer, and the next n bits is the number to be multiplied by pow(2,n)
 * The double equivalent will be the sum of those two results
 * TODO double check this conversion, write tests for it
 *
 * @param q97 a number in Q9.7 format
 * @return double
 */
double
ConvertQ97FormatToIEEE(uint16_t q97)
{
    static const double pow2 = std::pow(2, -7);
    static const uint16_t sign_mask =               0b1000'0000'0000'0000U;
    static const uint16_t unsigned_integer_mask =   0b0111'1111'1000'0000U;
    static const uint16_t fraction_mask = ~(sign_mask | unsigned_integer_mask);

    bool sign = q97 & sign_mask;
    int unsigned_integer_part = (q97 & unsigned_integer_mask) >> 7U;
    int fraction_part = q97 & fraction_mask;

    double unsigned_number = ((double)unsigned_integer_part) + (((double)fraction_part) * pow2);

    return (sign ? -1 : 1) * unsigned_number;
}

UwbPeer::UwbPeer(const uwb::protocol::fira::UwbRangingMeasurement& data) :
    m_address{ data.PeerMacAddress },
    m_spatialProperties{
        .Distance{ data.Distance }, // TODO is this also q97
        .AngleAzimuth{ ConvertQ9_7FormatToIEEE(data.AoAAzimuth.Result) },
        .AngleElevation{ ConvertQ9_7FormatToIEEE(data.AoAElevation.Result) },
        .Elevation{ ConvertQ9_7FormatToIEEE(data.AoaDestinationElevation.Result) }, // TODO is this right?

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
    ss << "{" << std::endl;
    ss << "mac: " << m_address << std::endl;
    ss << m_spatialProperties << std::endl;
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
