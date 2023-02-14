
#ifndef UWB_SIMULATOR_DDI_CALLBACKS_LRP_NOOP
#define UWB_SIMULATOR_DDI_CALLBACKS_LRP_NOOP

#include <cstdint>
#include <future>
#include <memory>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <unordered_map>
#include <vector>

#include "UwbSimulatorDdiCallbacksLrp.hxx"
#include "UwbSimulatorSession.hxx"

#include <uwb/protocols/fira/UwbApplicationConfiguration.hxx>
#include <uwb/protocols/fira/UwbCapability.hxx>
#include <windows/devices/uwb/UwbAppConfiguration.hxx>

namespace windows::devices::uwb::simulator
{
struct UwbSimulatorDdiCallbacksLrpNoop :
    public UwbSimulatorDdiCallbacksLrp
{
    virtual UwbStatus
    DeviceReset() override;

    virtual UwbStatus
    DeviceGetInformation(UwbDeviceInformation &deviceInfo) override;

    virtual UwbStatus
    DeviceGetCapabilities(UwbCapability &deviceCapabilities) override;

    virtual UwbStatus
    DeviceGetConfigurationParameters(std::vector<UwbDeviceConfigurationParameterType> &deviceConfigurationParameterTypes, std::vector<std::tuple<UwbDeviceConfigurationParameterType, UwbStatus, std::optional<UwbDeviceConfigurationParameter>>> &deviceConfigurationParameterResults) override;

    virtual UwbStatus
    DeviceSetConfigurationParameters(const std::vector<UwbDeviceConfigurationParameter> &deviceConfigurationParameters, std::vector<std::tuple<UwbDeviceConfigurationParameterType, UwbStatus>> &deviceConfigurationParameterResults) override;

    virtual UwbStatus
    SessionInitialize(uint32_t sessionId, UwbSessionType sessionType) override;

    virtual UwbStatus
    SessionDeninitialize(uint32_t sessionId) override;

    virtual UwbStatus
    SetApplicationConfigurationParameters(uint32_t sessionId, const std::vector<std::shared_ptr<IUwbAppConfigurationParameter>> &applicationConfigurationParameters, std::vector<std::tuple<UwbApplicationConfigurationParameterType, UwbStatus, std::shared_ptr<IUwbAppConfigurationParameter>>> &applicationConfigurationParameterResults) override;

    virtual UwbStatus
    GetApplicationConfigurationParameters(uint32_t sessionId, std::vector<std::shared_ptr<IUwbAppConfigurationParameter>> &applicationConfigurationParameters) override;

    virtual UwbStatus
    GetSessionCount(uint32_t &sessionCount) override;

    virtual UwbStatus
    SessionGetState(uint32_t sessionId, UwbSessionState &sessionState) override;

    virtual UwbStatus
    SessionUpdateControllerMulticastList(uint32_t sessionId, std::vector<UwbMacAddress> controlees) override;

    virtual UwbStatus
    SessionStartRanging(uint32_t sessionId) override;

    virtual UwbStatus
    SessionStopRanging(uint32_t sessionId) override;

    virtual UwbStatus
    SessionGetRangingCount(uint32_t sessionId, uint32_t &rangingCount) override;

    virtual void
    UwbNotification(UwbNotificationData &notificationData) override;

protected:
    void
    SessionUpdateState(UwbSimulatorSession &session, UwbSessionState sessionState);

private:
    // Static device information.
    UwbDeviceInformation m_deviceInformation{};
    UwbCapability m_deviceCapabilities{};

    // Session state and associated lock that protects it.
    std::shared_mutex m_sessionsGate;
    std::unordered_map<uint32_t, UwbSimulatorSession> m_sessions{};

    // Notification promise and associated lock that protects it.
    std::mutex m_notificationGate;
    std::optional<std::promise<UwbNotificationData>> m_notificationPromise;
};
} // namespace windows::devices::uwb::simulator

#endif // UWB_SIMULATOR_DDI_CALLBACKS_LRP_NOOP
