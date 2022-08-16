
#include <nearobject/NearObjectProfileSecurity.hxx>

bool
nearobject::NearObjectProfileSecurity::IsSame(const NearObjectProfileSecurity& other) const noexcept
{
    return true;
}

bool
nearobject::operator==(const NearObjectProfileSecurity& lhs, const NearObjectProfileSecurity& rhs) noexcept
{
    return lhs.IsSame(rhs);
}

bool
nearobject::operator!=(const NearObjectProfileSecurity& lhs, const NearObjectProfileSecurity& rhs) noexcept
{
    return !(lhs == rhs);
}
