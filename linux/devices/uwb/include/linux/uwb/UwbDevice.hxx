
#ifndef LINUX_DEVICE_UWB_HXX
#define LINUX_DEVICE_UWB_HXX

#include <cstdint>
#include <memory>

#include <uwb/UwbDevice.hxx>
#include <uwb/UwbSessionEventCallbacks.hxx>

namespace linux
{
namespace devices
{
class UwbDevice :
    public uwb::UwbDevice
{
public:
    /**
     * @brief Construct a new UwbDevice object.
     */
    UwbDevice() = default;

    /**
     * @brief Create a new UWB session.
     * 
     * @return std::unique_ptr<uwb::UwbSession> 
     */
    std::unique_ptr<uwb::UwbSession>
    CreateSession(std::weak_ptr<uwb::UwbSessionEventCallbacks> callbacks) override;

    /**
     * @brief Get the capabilities of the device.
     * 
     * @return uwb::protocol::fira::UwbCapability 
     */
    uwb::protocol::fira::UwbCapability
    GetCapabilities() const override;

    /**
     * @brief Determine if this device is the same as another.
     * 
     * @param other 
     * @return true 
     * @return false 
     */
    bool
    IsEqual(const uwb::UwbDevice& other) const noexcept override;
};

} // namespace devices
} // namespace linux

#endif // LINUX_DEVICE_UWB_HXX
