
#ifndef I_UWB_DEVICE_DDI_HXX
#define I_UWB_DEVICE_DDI_HXX

#include <cstdint>
#include <future>
#include <optional>
#include <tuple>
#include <vector>

#include <uwb/protocols/fira/FiraDevice.hxx>
#include <uwb/protocols/fira/UwbCapability.hxx>

namespace windows::devices::uwb
{
struct IUwbDeviceDdi
{
    // IOCTL_UWB_DEVICE_RESET
    virtual std::future<::uwb::protocol::fira::UwbStatus>
    Reset() = 0;

    // IOCTL_UWB_GET_DEVICE_INFO
    virtual std::future<::uwb::protocol::fira::UwbDeviceInformation>
    GetDeviceInformation() = 0;

    // IOCTL_UWB_GET_DEVICE_CAPABILITIES
    virtual std::future<std::tuple<::uwb::protocol::fira::UwbStatus, ::uwb::protocol::fira::UwbCapability>>
    GetCapabilities() = 0;

    // IOCTL_UWB_GET_SESSION_COUNT
    virtual std::future<::uwb::protocol::fira::UwbStatus>
    GetSessionCount(uint32_t &sessionCount) = 0;

    // IOCTL_UWB_SESSION_INIT
    virtual std::future<::uwb::protocol::fira::UwbStatus>
    SessionIntitialize(uint32_t sessionId, ::uwb::protocol::fira::UwbSessionType sessionType) = 0;
    
    // IOCTL_UWB_SESSION_DEINIT
    virtual std::future<::uwb::protocol::fira::UwbStatus>
    SessionDeinitialize(uint32_t sessionId) = 0;

    // IOCTL_UWB_GET_SESSION_STATE
    virtual std::future<std::tuple<::uwb::protocol::fira::UwbStatus, std::optional<::uwb::protocol::fira::UwbSessionState>>>
    SessionGetState(uint32_t sessionId) = 0;

    // IOCTL_UWB_START_RANGING_SESSION
    virtual std::future<::uwb::protocol::fira::UwbStatus>
    SessionRangingStart(uint32_t sessionId) = 0;

    // IOCTL_UWB_STOP_RANGING_SESSION
    virtual std::future<::uwb::protocol::fira::UwbStatus>
    SessionRangingStop(uint32_t sessionId) = 0;

    // IOCTL_UWB_GET_RANGING_COUNT
    virtual std::future<std::tuple<::uwb::protocol::fira::UwbStatus, std::optional<uint32_t>>>
    SessionGetRangingCount(uint32_t sessionId) = 0;

    // IOCTL_UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST
    virtual std::future<::uwb::protocol::fira::UwbSessionUpdateMulicastListStatus>
    SessionUpdateControllerMulticastList(uint32_t sessionId, ::uwb::protocol::fira::UwbMulticastAction multicastAction, std::vector<::uwb::UwbMacAddress> controlees) = 0;

    // TODO: unspecified IOCTLs below
    // IOCTL_UWB_SET_DEVICE_CONFIG_PARAMS
    // IOCTL_UWB_GET_DEVICE_CONFIG_PARAMS
    // IOCTL_UWB_SET_APP_CONFIG_PARAMS
    // IOCTL_UWB_GET_APP_CONFIG_PARAMS
};
} // namespace windows::devices::uwb

#endif // I_UWB_DEVICE_DDI_HXX
