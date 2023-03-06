
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
     * @brief Determine if this device is the same as another.
     * 
     * @param other 
     * @return true 
     * @return false 
     */
    bool
    IsEqual(const uwb::UwbDevice& other) const noexcept override;

private:
    /**
     * @brief Create a Session object
     * 
     * @param callbacks 
     * @return std::shared_ptr<uwb::UwbSession> 
     */
    std::shared_ptr<uwb::UwbSession>
    CreateSessionImpl(std::weak_ptr<uwb::UwbSessionEventCallbacks> callbacks) override;

    /**
     * @brief Get the capabilities of the device.
     * 
     * @return uwb::protocol::fira::UwbCapability 
     */
    uwb::protocol::fira::UwbCapability
    GetCapabilitiesImpl() override;

    /**
     * @brief Get the FiRa device information of the device.
     * 
     * @return uwb::protocol::fira::UwbDeviceInformation 
     */
    uwb::protocol::fira::UwbDeviceInformation
    GetDeviceInformationImpl() override;

    /**
     * @brief Reset the device to an initial clean state. 
     */
    void
    ResetImpl() override;
};

} // namespace devices
} // namespace linux

#endif // LINUX_DEVICE_UWB_HXX
