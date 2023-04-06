
#ifndef NEAR_OBJECT_CAPABILITIES_HXX
#define NEAR_OBJECT_CAPABILITIES_HXX

#include <tuple>

namespace nearobject
{
/**
 * @brief Describes the capabilities of a near object.
 */
struct NearObjectCapabilities
{
    /**
     * @brief Supports ranging or distance measurements.
     */
    bool SupportsRanging;

    /**
     * @brief Supports positioning measurements (ranging + angle).
     */
    bool SupportsPositioning;

    /**
     * @brief Supports one or more attached secure devices.
     */
    bool SupportsSecureDevice;

    /**
     * @brief Supports creation of secure channels.
     */
    bool SupportsSecureChannels;

    auto
    operator<=>(const NearObjectCapabilities& other) const = default;
};
} // namespace nearobject

#endif // NEAR_OBJECT_CAPABILITIES_HXX