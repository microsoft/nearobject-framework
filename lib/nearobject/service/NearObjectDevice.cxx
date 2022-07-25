
#include "NearObjectDevice.hxx"

using namespace nearobject::service;

bool
NearObjectDevice::operator==(const NearObjectDevice& other) const noexcept
{
    return (*this == other);
}

bool
NearObjectDevice::IsSame(const NearObjectDevice& other) const noexcept
{
    return IsSameImpl(other);
}

bool
NearObjectDevice::IsSameImpl(const NearObjectDevice& other) const noexcept
{
    // Default to member-wise comparison if not overriden.
    return (*this == other);
}
