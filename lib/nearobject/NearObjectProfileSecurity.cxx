
#include <nearobject/NearObjectProfileSecurity.hxx>

bool
nearobject::NearObjectConnectionProfileSecurity::IsSame(const NearObjectConnectionProfileSecurity& other) const noexcept
{
    return true;
}

bool
nearobject::operator==(const NearObjectConnectionProfileSecurity& lhs, const NearObjectConnectionProfileSecurity& rhs) noexcept
{
    return lhs.IsSame(rhs);
}

bool
nearobject::operator!=(const NearObjectConnectionProfileSecurity& lhs, const NearObjectConnectionProfileSecurity& rhs) noexcept
{
    return !(lhs == rhs);
}
