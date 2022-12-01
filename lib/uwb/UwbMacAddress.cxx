
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
    m_type(other.m_type),
    m_length(other.m_length),
    m_value(other.m_value)
{
    // Note that the view span (m_view) cannot be directly copied from the other
    // instance because that view refers to its own address storage. Hence, the
    // view is explicitly initialized here.
    InitializeView();
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
    std::swap(this->m_view, other.m_view);
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

std::string
UwbMacAddress::ToString() const
{
    std::ostringstream macString{};

    macString << magic_enum::enum_name(m_type) << ' ' << std::hex;
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
