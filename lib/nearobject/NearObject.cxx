
#include <tuple>

#include <nearobject/NearObject.hxx>

using namespace nearobject;

NearObjectSpatialProperties
NearObject::GetSpatialProperties() const noexcept
{
    const auto lock = std::scoped_lock{ m_spatialPropertiesGate };
    return m_spatialProperties;
}

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

bool
nearobject::operator==(const NearObjectSpatialProperties& lhs, const NearObjectSpatialProperties& rhs) noexcept
{
    return std::tie(lhs.Distance, lhs.AngleAzimuth, lhs.AngleElevation, lhs.Elevation)
        == std::tie(rhs.Distance, rhs.AngleAzimuth, rhs.AngleElevation, rhs.Elevation);
}
