
#include <iomanip>
#include <ios>
#include <sstream>
#include <stdexcept>

#include <magic_enum.hpp>

#include <uwb/UwbMacAddress.hxx>

using namespace uwb;

UwbMacAddress::UwbMacAddress() :
    UwbMacAddress(ShortType{ 0x00, 0x00 })
{}

UwbMacAddress::UwbMacAddress(const UwbMacAddress& other) :
    m_length(other.m_length),
    m_type(other.m_type),
    m_value(other.m_value)
{
    // Note that the view span (m_view) cannot be directly copied from the other
    // instance because that view refers to its own address storage. Hence, the
    // view is explicitly initialized here.
    InitializeView();
}

UwbMacAddress::UwbMacAddress(std::string addressString, UwbMacAddressType addressType)
{
    std::array<uint8_t, ShortLength> shortAddress{};
    std::array<uint8_t, ExtendedLength> extendedAddress{};

    std::stringstream ss(addressString);
    for (auto i = 0; i < (addressType == UwbMacAddressType::Short ? ShortLength : ExtendedLength); i++) {
        if (ss) {
            std::string addressByte;
            getline(ss, addressByte, ':');
            const auto byteValue = static_cast<uint8_t>(std::stoi(addressByte));
            if (addressType == UwbMacAddressType::Short) {
                shortAddress[i] = byteValue;
            } else {
                extendedAddress[i] = byteValue;
            }
        } else {
            break;
        }
    }

    if (addressType == UwbMacAddressType::Short) {
        *this = UwbMacAddress(shortAddress);
    } else {
        *this = UwbMacAddress(extendedAddress);
    }
}

UwbMacAddress&
UwbMacAddress::operator=(UwbMacAddress other)
{
    // Note that this implementation of operator= uses a copy of another
    // instance, whereas the typical implementation takes a const reference.
    // This value-based version is used to ensure copy-elision occurs, avoiding
    // the need to create a temporary when using the standard copy-and-swap
    // idiom.
    Swap(other);
    return *this;
}

void
UwbMacAddress::InitializeView()
{
    // clang-format off
    std::visit([&](auto&& value) {
        m_view = { std::begin(value), std::end(value) };
    }, m_value);
    // clang-format on
}

void
UwbMacAddress::Swap(UwbMacAddress& other) noexcept
{
    std::swap(this->m_type, other.m_type);
    std::swap(this->m_length, other.m_length);
    std::swap(this->m_value, other.m_value);
    InitializeView();
    other.InitializeView();
}

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
