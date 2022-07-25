
#ifndef __NEAR_OBJECT_DEVICE_UWB_HXX__
#define __NEAR_OBJECT_DEVICE_UWB_HXX__

#include <uwb/UwbDevice.hxx>

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
public:
    explicit NearObjectDeviceUwb(std::unique_ptr<uwb::UwbDevice> uwbDevice);

private:
    StartSessionResult
    StartSessionImpl(const NearObjectConnectionProfile& profile) override;

private:
    std::unique_ptr<uwb::UwbDevice> m_uwbDevice{};
};

} // namespace service
} // namespace nearobject

#endif // _NEAR_OBJECT_DEVICE_UWB_HXX__
