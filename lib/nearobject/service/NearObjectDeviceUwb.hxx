
#ifndef NEAR_OBJECT_DEVICE_UWB_HXX
#define NEAR_OBJECT_DEVICE_UWB_HXX

#include <cstdint>
#include <memory>

#include <uwb/UwbDevice.hxx>

#include "NearObjectDevice.hxx"

namespace nearobject
{
struct NearObjectSessionEventCallbacks;

namespace service
{
/**
 * @brief A device providing Near Object services using a UWB device.
 */
class NearObjectDeviceUwb :
    public NearObjectDevice
{
public:
    NearObjectDeviceUwb(std::unique_ptr<uwb::UwbDevice> uwbDevice);

private:
    StartSessionResult
    StartSessionImpl(const NearObjectProfile& profile, std::weak_ptr<NearObjectSessionEventCallbacks> eventCallbacks) override;

private:
    std::unique_ptr<uwb::UwbDevice> m_uwbDevice{};
};

} // namespace service
} // namespace nearobject

#endif // _NEAR_OBJECT_DEVICE_UWB_HXX
