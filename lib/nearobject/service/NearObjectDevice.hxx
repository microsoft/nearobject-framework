
#ifndef __NEAR_OBJECT_DEVICE_HXX__
#define __NEAR_OBJECT_DEVICE_HXX__

namespace nearobject
{
namespace service
{
/**
 * @brief Represents a device which provides Near Object services.
 */
class NearObjectDevice
{
public:
    // Disable copy and move operations
    NearObjectDevice(NearObjectDevice&) = delete;
    NearObjectDevice(NearObjectDevice&&) = delete;
    NearObjectDevice& operator=(NearObjectDevice&) = delete;
    NearObjectDevice& operator=(NearObjectDevice&&) = delete;
};

} // namespace service
} // namespace nearobject

#endif // _NEAR_OBJECT_DEVICE_HXX__
