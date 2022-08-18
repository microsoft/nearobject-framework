
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
nearobject::operator==(const NearObject& lhs, const NearObject& rhs) noexcept
{
    return std::tie(lhs.m_spatialProperties) 
        == std::tie(rhs.m_spatialProperties); 
}

bool
nearobject::operator!=(const NearObject& lhs, const NearObject& rhs) noexcept
{
    return !(lhs == rhs);
}

bool
nearobject::operator==(const NearObjectSpatialProperties& lhs, const NearObjectSpatialProperties& rhs) noexcept
{
    return std::tie(lhs.Distance, lhs.AngleAzimuth, lhs.AngleElevation, lhs.Elevation)
        == std::tie(rhs.Distance, rhs.AngleAzimuth, rhs.AngleElevation, rhs.Elevation);
}

bool
nearobject::operator!=(const NearObjectSpatialProperties& lhs, const NearObjectSpatialProperties& rhs) noexcept
{
    return !(lhs == rhs);
}
