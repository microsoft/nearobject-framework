
#ifndef __NEAR_OBJECT_CAPABILITIES_HXX__
#define __NEAR_OBJECT_CAPABILITIES_HXX__

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

    /**
     * @brief Implement equality using member-wise comparison.
     *
     * @param other
     * @return true
     * @return false
     */
    bool
    operator==(const NearObjectCapabilities& other) const noexcept
    {
        return std::tie(this->SupportsRanging, this->SupportsPositioning, this->SupportsSecureDevice, this->SupportsSecureChannels)
            == std::tie(other.SupportsRanging, other.SupportsPositioning, other.SupportsSecureDevice, other.SupportsSecureChannels);
    }
};
} // namespace nearobject

#endif // __NEAR_OBJECT_CAPABILITIES_HXX__