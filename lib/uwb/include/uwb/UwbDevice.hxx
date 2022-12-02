
#ifndef UWB_DEVICE_HXX
#define UWB_DEVICE_HXX

#include <memory>

#include <uwb/protocols/fira/UwbCapability.hxx>
#include <uwb/UwbSessionEventCallbacks.hxx>

namespace uwb
{
class UwbSession;

/**
 * @brief Interface for interacting with a UWB device.
 */
class UwbDevice
{
public:
    /**
     * @brief Creates a new UWB session with no configuration nor peers.
     * 
     * @return std::unique_ptr<uwb::UwbSession> 
     */
    virtual std::unique_ptr<UwbSession>
    CreateSession(std::weak_ptr<UwbSessionEventCallbacks> callbacks) = 0;

    /**
     * @brief Get the FiRa capabilities of the device.
     * 
     * @return uwb::protocol::fira::UwbCapability 
     */
    virtual uwb::protocol::fira::UwbCapability
    GetCapabilities() const = 0;

    /**
     * @brief Determine if this device is the same as another.
     * 
     * @param other 
     * @return true 
     * @return false 
     */
    virtual bool
    IsEqual(const UwbDevice& other) const noexcept = 0;

    /**
     * @brief Destroy the UwbDevice object.
     */
    virtual ~UwbDevice() = default;
};

bool
operator==(const UwbDevice&, const UwbDevice&) noexcept;

bool
operator!=(const UwbDevice&, const UwbDevice&) noexcept;

} // namespace uwb

#endif // UWB_DEVICE_HXX
