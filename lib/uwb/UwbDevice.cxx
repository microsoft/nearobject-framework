
#include <uwb/UwbDevice.hxx>

bool
uwb::operator==(const UwbDevice& lhs, const UwbDevice& rhs) noexcept
{
    return (typeid(lhs) == typeid(rhs)) && lhs.IsEqual(rhs);
}

bool
uwb::operator!=(const UwbDevice& lhs, const UwbDevice& rhs) noexcept
{
    return !(lhs == rhs);
}
