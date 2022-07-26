
#include "NearObject.hxx"

using namespace nearobject;

bool
NearObject::IsSame(const NearObject& other) const noexcept
{
    // TODO: update this once this class has data members.
    return (this == &other);
}

bool
nearobject::operator==(const NearObject& lhs, const NearObject& rhs) noexcept
{
    return lhs.IsSame(rhs);
}
