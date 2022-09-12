
#ifndef NEAR_OBJECT_DEVICE_CONTROLLER_UWB_HXX
#define NEAR_OBJECT_DEVICE_CONTROLLER_UWB_HXX

#include <cstdint>
#include <memory>

#include <uwb/UwbDevice.hxx>

#include <nearobject/service/NearObjectDeviceController.hxx>

namespace nearobject
{
struct NearObjectSessionEventCallbacks;

namespace service
{
/**
 * @brief A device providing Near Object services using a UWB device.
 */
class NearObjectDeviceControllerUwb :
    public NearObjectDeviceController
{
public:
    NearObjectDeviceControllerUwb(std::unique_ptr<uwb::UwbDevice> uwbDevice);

private:
    StartSessionResult
    StartSessionImpl(const NearObjectProfile& profile, std::weak_ptr<NearObjectSessionEventCallbacks> eventCallbacks) override;

private:
    std::unique_ptr<uwb::UwbDevice> m_uwbDevice{};
};

} // namespace service
} // namespace nearobject

#endif // NEAR_OBJECT_DEVICE_CONTROLLER_UWB_HXX
