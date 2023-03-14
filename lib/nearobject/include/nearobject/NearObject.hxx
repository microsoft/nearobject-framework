
#ifndef NEAR_OBJECT_HXX
#define NEAR_OBJECT_HXX

#include <memory>
#include <mutex>
#include <optional>

#include <nearobject/NearObjectIdentityToken.hxx>

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
     * @brief Construct a new NearObject with an identity token.
     * 
     * @param identityToken The identityToken to initialize.
     */
    explicit NearObject(std::shared_ptr<NearObjectIdentityToken> identityToken);

    /**
     * @brief Retrieves the identity token of this near object.
     * 
     * @return std::shared_ptr<NearObjectIdentityToken> 
     */
    std::shared_ptr<NearObjectIdentityToken>
    GetIdentityToken() const noexcept;

    /**
     * @brief Retrieves the latest spatial properties for this near object.
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
    std::shared_ptr<NearObjectIdentityToken> m_identityToken;
};

bool
operator==(const NearObject&, const NearObject&) noexcept;

bool
operator!=(const NearObject&, const NearObject&) noexcept;

} // namespace nearobject

#endif // NEAR_OBJECT_HXX
