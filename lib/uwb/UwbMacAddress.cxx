
#include <iomanip>
#include <ios>
#include <sstream>
#include <stdexcept>

#include <magic_enum.hpp>

#include <uwb/UwbMacAddress.hxx>

using namespace uwb;

UwbMacAddressType
UwbMacAddress::GetType() const noexcept
{
    return m_type;
}

std::size_t
UwbMacAddress::GetLength() const noexcept
{
    return m_length;
}

std::span<const uint8_t>
UwbMacAddress::GetValue() const noexcept
{
    return m_view;
}

std::optional<uint16_t>
UwbMacAddress::GetValueShort() const
{
    if (m_type != UwbMacAddressType::Short) {
        return std::nullopt;
    }

    // TODO: do we need to account for endianness? revisit
    return (static_cast<uint16_t>(m_view[1]) << 8U) | m_view[0];
}

/* static */
UwbMacAddress
UwbMacAddress::Random(UwbMacAddressType type)
{
    switch (type) {
    case UwbMacAddressType::Short:
        return Random<UwbMacAddressType::Short>();
    case UwbMacAddressType::Extended:
        return Random<UwbMacAddressType::Extended>();
    default:
        throw std::runtime_error("unknown mac address type");
    }
}

std::string
UwbMacAddress::ToString() const
{
    std::ostringstream macString{};

    for (const auto& b : m_view.first(m_view.size() - 1)) {
        macString << +b << ':';
    }
    macString << +m_view[m_view.size() - 1];

    return macString.str();
}

bool
uwb::operator==(const UwbMacAddress& lhs, const UwbMacAddress& rhs) noexcept
{
    return (lhs.m_value == rhs.m_value);
}

auto
uwb::UwbMacAddress::operator<=>(const UwbMacAddress& other) const noexcept
{
    return this->m_value <=> other.m_value;
}

std::istream&
uwb::operator>>(std::istream& stream, UwbMacAddress& uwbMacAddress) noexcept
{
    char separator = ':';
    std::size_t numOctets;
    std::array<uint16_t, 8> data{};

    // Set stream numeric base to hexadecimal.
    stream >> std::setbase(16);

    for (numOctets = 0; numOctets < data.size(); numOctets++) {
        stream >> data[numOctets];
        if (numOctets != data.size() - 1) {
            stream >> separator;
        }
        if (stream.fail()) {
            break;
        }
    }

    if (numOctets == data.size()) {
        uwbMacAddress = UwbMacAddress{ std::array<uint8_t, 8>{
            static_cast<uint8_t>(data[0] & 0x000000FFu),
            static_cast<uint8_t>(data[1] & 0x000000FFu),
            static_cast<uint8_t>(data[2] & 0x000000FFu),
            static_cast<uint8_t>(data[3] & 0x000000FFu),
            static_cast<uint8_t>(data[4] & 0x000000FFu),
            static_cast<uint8_t>(data[5] & 0x000000FFu),
            static_cast<uint8_t>(data[6] & 0x000000FFu),
            static_cast<uint8_t>(data[7] & 0x000000FFu) } };
    } else if (numOctets > 0) {
        stream.clear();
        uwbMacAddress = UwbMacAddress{ std::array<uint8_t, 2>{
            static_cast<uint8_t>(data[0] & 0x000000FFu),
            static_cast<uint8_t>(data[1] & 0x000000FFu) } };
    }

    return stream;
}
