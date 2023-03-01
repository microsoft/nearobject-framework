
#ifndef UWB_SIMULATOR_DDI_CALLBACKS_HXX
#define UWB_SIMULATOR_DDI_CALLBACKS_HXX

#include <cstdint>
#include <future>
#include <memory>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <unordered_map>
#include <vector>

#include <windows.h>

#include <wdf.h>

#include "UwbSimulatorDdi.h"

#include "IUwbSimulatorDdiCallbacksLrp.hxx"
#include "IUwbSimulatorDdiCallbacksSimulator.hxx"
#include "UwbSimulatorSession.hxx"

#include <uwb/protocols/fira/UwbApplicationConfiguration.hxx>
#include <uwb/protocols/fira/UwbCapability.hxx>
#include <windows/devices/uwb/UwbAppConfiguration.hxx>

namespace windows::devices::uwb::simulator
{
struct UwbSimulatorDdiCallbacks :
    public IUwbSimulatorDdiCallbacksLrp,
    public IUwbSimulatorDdiCallbacksSimulator
{
    UwbSimulatorDdiCallbacks();

    // IUwbSimulatorDdiCallbacksLrp

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

    virtual NTSTATUS
    UwbNotification(UwbNotificationData &notificationData) override;

    // IUwbSimulatorDdiCallbacksSimulator

    virtual UwbSimulatorCapabilities
    GetSimulatorCapabilities() override;

    virtual void
    TriggerSessionEvent(const UwbSimulatorTriggerSessionEventArgs &triggerSessionEventArgs) override;

protected:
    /**
     * @brief Update the state of the specified session.
     *
     * This function will also generate a UWB notification associated with the change.
     *
     * @param session The session to update.
     * @param sessionState The new session state.
     * @param reasonCode The reason code for the update, if sessionState == UwbSessionState::Idle.
     */
    void
    SessionUpdateState(UwbSimulatorSession &session, UwbSessionState sessionState, std::optional<UwbSessionReasonCode> reasonCode);

    /**
     * @brief Raise a UWB notification.
     *
     * @param uwbNotificationData The notification data to provide with the event.
     */
    NTSTATUS
    RaiseUwbNotification(UwbNotificationData uwbNotificationData);

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

private:
    UwbSimulatorCapabilities m_simulatorCapabilities{};
};
} // namespace windows::devices::uwb::simulator

#endif // UWB_SIMULATOR_DDI_CALLBACKS_HXX
