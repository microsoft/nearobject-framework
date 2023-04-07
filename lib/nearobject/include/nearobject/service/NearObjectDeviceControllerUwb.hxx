
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
    /**
     * @brief Construct a new NearObjectDeviceControllerUwb object.
     *
     * @param uwbDevice The underlying UWB device.
     */
    NearObjectDeviceControllerUwb(std::shared_ptr<uwb::UwbDevice> uwbDevice);

    /**
     * @brief
     *
     * @param other
     * @return true
     * @return false
     */
    bool
    IsEqual(const NearObjectDeviceController& other) const noexcept override;

private:
    StartSessionResult
    StartSessionImpl(const NearObjectProfile& profile, std::weak_ptr<NearObjectSessionEventCallbacks> eventCallbacks) override;

private:
    std::shared_ptr<::uwb::UwbDevice> m_uwbDevice{};
};

} // namespace service
} // namespace nearobject

#endif // NEAR_OBJECT_DEVICE_CONTROLLER_UWB_HXX
