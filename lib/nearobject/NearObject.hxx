
#ifndef __NEAR_OBJECT_HXX__
#define __NEAR_OBJECT_HXX__

#include <mutex>
#include <optional>

namespace nearobject
{
/**
 * @brief Decribes the spatial properties of a near object. All property values
 * are relative to the hosting device.
 * 
 * TODO: Select and document coordinate system
 * TODO: Select and document units
 */
struct NearObjectSpatialProperties 
{
    std::optional<double> Distance;
    std::optional<double> AngleAzimuth;
    std::optional<double> AngleElevation;
    std::optional<double> Elevation;
};

bool
operator==(const NearObjectSpatialProperties&, const NearObjectSpatialProperties&) noexcept;

/**
 * @brief Represents a near object.
 */
class NearObject
{
public:
    /**
     * @brief Retrieves the latest spatial properties for these near object.
     * 
     * @return NearObjectSpatialProperties 
     */
    NearObjectSpatialProperties
    GetSpatialProperties() const noexcept;

    /**
     * @brief Determines if the specified near object refers to the same entity
     * as this near object. 
     * 
     * @param other
     * @return true 
     * @return false 
     */
    bool
    IsSame(const NearObject& other) const noexcept;

private:
    mutable std::mutex m_spatialPropertiesGate;
    NearObjectSpatialProperties m_spatialProperties{};
};

bool
operator==(const NearObject&, const NearObject&) noexcept;

} // namespace nearobject

#endif // __NEAR_OBJECT_HXX__
