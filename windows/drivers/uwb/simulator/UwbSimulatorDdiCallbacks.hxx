
#ifndef UWB_SIMULATOR_DDI_CALLBACKS_HXX
#define UWB_SIMULATOR_DDI_CALLBACKS_HXX

#include <cstdint>
#include <memory>
#include <mutex>
#include <optional>
#include <vector>

#include <windows.h>

#include <wdf.h>

#include "UwbSimulatorDdi.h"

#include "IUwbSimulatorDdiCallbacksLrp.hxx"
#include "IUwbSimulatorDdiCallbacksSimulator.hxx"
#include "UwbSimulatorDevice.hxx"
#include "UwbSimulatorSession.hxx"

#include <uwb/protocols/fira/UwbCapability.hxx>

namespace windows::devices::uwb::simulator
{
struct UwbSimulatorDdiCallbacks :
    public IUwbSimulatorDdiCallbacksLrp,
    public IUwbSimulatorDdiCallbacksSimulator
{
    UwbSimulatorDdiCallbacks(UwbSimulatorDevice *device);

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
    SetApplicationConfigurationParameters(uint32_t sessionId, std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameter> &uwbApplicationConfigurationParameters, std::vector<::uwb::protocol::fira::UwbSetApplicationConfigurationParameterStatus> &uwbSetApplicationConfigurationParameterStatuses) override;

    virtual UwbStatus
    GetApplicationConfigurationParameters(uint32_t sessionId, std::vector<UwbApplicationConfigurationParameterType> applicationConfigurationParameterTypes, std::vector<UwbApplicationConfigurationParameter> &applicationConfigurationParameters) override;

    virtual UwbStatus
    GetSessionCount(uint32_t &sessionCount) override;

    virtual UwbStatus
    SessionGetState(uint32_t sessionId, UwbSessionState &sessionState) override;

    virtual UwbStatus
    SessionUpdateControllerMulticastList(uint32_t sessionId, UwbMulticastAction action, std::vector<UwbSessionUpdateMulticastListEntry> updateMulticastListEntries) override;

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
     * @brief Obtain a reference to the session context for the specified
     * session id.
     *
     * @param sessionId The session id to obtain session context for.
     * @return std::tuple<UwbStatus, std::shared_ptr<UwbSimulatorSession>>
     */
    std::tuple<UwbStatus, std::shared_ptr<UwbSimulatorSession>>
    SessionGet(uint32_t sessionId);

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
     * @brief Configure a session for random measurement generation.
     *
     * @param sessionId The session to configure.
     * @param action The action to take.
     */
    void
    SessionRandomMeasurementGenerationConfigure(uint32_t sessionId, RandomMeasurementGeneration action);

    /**
     * @brief Raise a UWB notification.
     *
     * @param uwbNotificationData The notification data to provide with the event.
     */
    void
    RaiseUwbNotification(UwbNotificationData uwbNotificationData);

private:
    // Static device information.
    UwbDeviceInformation m_deviceInformation{};
    UwbCapability m_deviceCapabilities{};

private:
    UwbSimulatorCapabilities m_simulatorCapabilities{};
    UwbSimulatorDevice *m_device{ nullptr };
};
} // namespace windows::devices::uwb::simulator

#endif // UWB_SIMULATOR_DDI_CALLBACKS_HXX
