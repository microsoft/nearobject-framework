
#ifndef NEAR_OBJECT_HXX
#define NEAR_OBJECT_HXX

#include <mutex>
#include <optional>

namespace nearobject
{
/**
 * @brief Describes the spatial properties of a near object. All property values
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

bool
operator!=(const NearObjectSpatialProperties&, const NearObjectSpatialProperties&) noexcept;

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
     * @brief Allow global equality function to access private members.
     * 
     * @return true 
     * @return false 
     */
    friend bool
    operator==(const NearObject&, const NearObject&) noexcept;

private:
    mutable std::mutex m_spatialPropertiesGate;
    NearObjectSpatialProperties m_spatialProperties{};
};

bool
operator==(const NearObject&, const NearObject&) noexcept;

bool
operator!=(const NearObject&, const NearObject&) noexcept;

} // namespace nearobject

#endif // NEAR_OBJECT_HXX
