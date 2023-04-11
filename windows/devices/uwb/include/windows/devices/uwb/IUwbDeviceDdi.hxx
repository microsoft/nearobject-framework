
#ifndef I_UWB_DEVICE_DDI_HXX
#define I_UWB_DEVICE_DDI_HXX

#include <cstdint>
#include <future>
#include <memory>
#include <tuple>
#include <vector>

#include <uwb/UwbRegisteredCallbacks.hxx>
#include <uwb/protocols/fira/FiraDevice.hxx>
#include <uwb/protocols/fira/UwbCapability.hxx>

namespace windows::devices::uwb
{
/**
 * @brief structure containing wrappers for exactly the DDI ioctls that UwbDevice should call
 *
 */
struct IUwbDeviceDdi
{
    // IOCTL_UWB_DEVICE_RESET
    virtual std::future<void>
    Reset() = 0;

    // IOCTL_UWB_GET_DEVICE_INFO
    virtual std::future<::uwb::protocol::fira::UwbDeviceInformation>
    GetDeviceInformation() = 0;

    // IOCTL_UWB_GET_DEVICE_CAPABILITIES
    virtual std::future<std::tuple<::uwb::protocol::fira::UwbStatus, ::uwb::protocol::fira::UwbCapability>>
    GetCapabilities() = 0;

    // IOCTL_UWB_GET_SESSION_COUNT
    virtual std::future<std::tuple<::uwb::protocol::fira::UwbStatus, uint32_t>>
    GetSessionCount() = 0;

    // TODO: unspecified IOCTLs below
    // IOCTL_UWB_SET_DEVICE_CONFIG_PARAMS
    // IOCTL_UWB_GET_DEVICE_CONFIG_PARAMS
};

struct IUwbDeviceDdiConnector : public IUwbDeviceDdi
{
    /**
     * @brief Sets the callbacks for the UwbDevice that owns this UwbConnector
     *
     * @param callbacks
     * @return RegisteredCallbackToken* You can pass this pointer into DeregisterEventCallback to deregister this event callback
     */
    virtual std::weak_ptr<::uwb::RegisteredCallbackToken> RegisterDeviceEventCallbacks(std::weak_ptr<::uwb::UwbRegisteredDeviceEventCallbacks>) = 0;
};

} // namespace windows::devices::uwb

#endif // I_UWB_DEVICE_DDI_HXX
