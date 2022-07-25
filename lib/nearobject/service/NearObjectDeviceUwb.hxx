
#ifndef __NEAR_OBJECT_DEVICE_UWB_HXX__
#define __NEAR_OBJECT_DEVICE_UWB_HXX__

#include "NearObjectDevice.hxx"

namespace nearobject
{
namespace service
{
/**
 * @brief A device providing Near Object services using UWB devices.
 */
class NearObjectDeviceUwb :
    public NearObjectDevice
{
private:
    StartSessionResult
    StartSessionImpl(const NearObjectConnectionProfile& profile) override;
};

} // namespace service
} // namespace nearobject

#endif // _NEAR_OBJECT_DEVICE_UWB_HXX__
