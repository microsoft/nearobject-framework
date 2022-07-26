
#ifndef __NEAR_OBJECT_CAPABILITIES_HXX__
#define __NEAR_OBJECT_CAPABILITIES_HXX__

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
};
}

#endif // __NEAR_OBJECT_CAPABILITIES_HXX__