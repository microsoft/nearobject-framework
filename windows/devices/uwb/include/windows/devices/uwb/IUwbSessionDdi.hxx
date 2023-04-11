
#ifndef I_UWB_SERVICE_DDI_HXX
#define I_UWB_SERVICE_DDI_HXX

#include <cstdint>
#include <future>
#include <memory>
#include <optional>
#include <tuple>
#include <vector>

#include <uwb/UwbRegisteredCallbacks.hxx>
#include <uwb/protocols/fira/FiraDevice.hxx>
#include <uwb/protocols/fira/UwbCapability.hxx>

namespace windows::devices::uwb
{
/**
 * @brief structure containing wrappers for exactly the DDI ioctls that UwbSession should call
 *
 */
struct IUwbSessionDdi
{
    // IOCTL_UWB_SESSION_INIT
    virtual std::future<::uwb::protocol::fira::UwbStatus>
    SessionInitialize(uint32_t sessionId, ::uwb::protocol::fira::UwbSessionType sessionType) = 0;

    // IOCTL_UWB_SESSION_DEINIT
    virtual std::future<::uwb::protocol::fira::UwbStatus>
    SessionDeinitialize(uint32_t sessionId) = 0;

    // IOCTL_UWB_GET_SESSION_STATE
    virtual std::future<std::tuple<::uwb::protocol::fira::UwbStatus, ::uwb::protocol::fira::UwbSessionState>>
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

    // IOCTL_UWB_GET_APP_CONFIG_PARAMS
    virtual std::future<std::tuple<::uwb::protocol::fira::UwbStatus, std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameter>>>
    GetApplicationConfigurationParameters(uint32_t sessionId, std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameterType> applicationConfigurationParameterTypes) = 0;

    // IOCTL_UWB_SET_APP_CONFIG_PARAMS
    virtual std::future<std::tuple<::uwb::protocol::fira::UwbStatus, std::vector<::uwb::protocol::fira::UwbSetApplicationConfigurationParameterStatus>>>
    SetApplicationConfigurationParameters(uint32_t sessionId, std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameter> applicationConfigurationParameters) = 0;
};

struct IUwbSessionDdiConnector : public IUwbSessionDdi
{
    /**
     * @brief Registers the callbacks for a particular session
     *
     * @param sessionId
     * @param callbacks
     * @return RegisteredCallbackToken* You can pass this pointer into DeregisterEventCallback to deregister this event callback
     */
    virtual std::weak_ptr<RegisteredCallbackToken>
    RegisterSessionEventCallbacks(uint32_t sessionId, std::weak_ptr<::uwb::UwbRegisteredSessionEventCallbacks> callbacks) = 0;
};

} // namespace windows::devices::uwb

#endif // I_UWB_SERVICE_DDI_HXX
