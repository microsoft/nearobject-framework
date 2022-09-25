
#include <stdexcept>

#include <uwb/UwbMacAddress.hxx>

using namespace uwb;

UwbMacAddress::UwbMacAddress() :
    UwbMacAddress(ShortType{ 0x00, 0x00 })
{}

UwbMacAddress::UwbMacAddress(UwbMacAddress::ShortType addressShort) :
    m_value(addressShort),
    Type(UwbMacAddressType::Short),
    Length(ShortLength)
{
    InitializeValue<ShortType>();
}

UwbMacAddress::UwbMacAddress(UwbMacAddress::ExtendedType addressExtended) :
    m_value(addressExtended),
    Type(UwbMacAddressType::Extended),
    Length(ExtendedLength)
{
    InitializeValue<ExtendedType>();
}

template <typename ActiveType> 
    requires (std::is_same_v<ActiveType, UwbMacAddress::ShortType> || std::is_same_v<ActiveType, UwbMacAddress::ExtendedType>)
void 
UwbMacAddress::InitializeValue()
{
    auto& value = std::get<ActiveType>(m_value);
    Value = { std::begin(value), std::end(value) };
}

std::array<uint8_t, UwbMacAddress::ShortLength>
UwbMacAddress::GetShort() const
{
    return std::get<decltype(GetShort())>(m_value);
}

std::array<uint8_t, UwbMacAddress::ExtendedLength>
UwbMacAddress::GetExtended() const
{
    return std::get<decltype(GetExtended())>(m_value);
}

bool
uwb::operator==(const UwbMacAddress& lhs, const UwbMacAddress& rhs) noexcept
{
    return (lhs.m_value == rhs.m_value);
}

bool
uwb::operator!=(const UwbMacAddress& lhs, const UwbMacAddress& rhs) noexcept
{
    return !(lhs == rhs);
}
