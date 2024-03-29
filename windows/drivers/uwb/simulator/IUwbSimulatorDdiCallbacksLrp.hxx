
#ifndef I_UWB_SIMULATOR_DDI_CALLBACKS_LRP
#define I_UWB_SIMULATOR_DDI_CALLBACKS_LRP

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include <windows.h>

#include <wdf.h>

#include <uwb/protocols/fira/FiraDevice.hxx>
#include <uwb/protocols/fira/UwbCapability.hxx>
#include <windows/devices/uwb/UwbCxAdapterDdiLrp.hxx>

using namespace uwb::protocol::fira;

namespace windows::devices::uwb::simulator
{
/**
 * @brief
 */
struct IUwbSimulatorDdiCallbacksLrp
{
    virtual ~IUwbSimulatorDdiCallbacksLrp() = default;

    /**
     * @brief Reset the UWB device.
     *
     * @return UwbStatus
     */
    virtual UwbStatus
    DeviceReset() = 0;

    /**
     * @brief Get uwb device information.
     *
     * @param deviceInfo
     * @return UwbStatus
     */
    virtual UwbStatus
    DeviceGetInformation(UwbDeviceInformation &deviceInfo) = 0;

    /**
     * @brief Get uwb device capabilities.
     *
     * @param deviceCapabilities
     * @return UwbStatus
     */
    virtual UwbStatus
    DeviceGetCapabilities(UwbCapability &deviceCapabilities) = 0;

    /**
     * @brief Get uwb device configuration parameters.
     *
     * @param deviceConfigurationParameterTypes
     * @param deviceConfigurationParameterResults
     * @return UwbStatus
     */
    virtual UwbStatus
    DeviceGetConfigurationParameters(std::vector<UwbDeviceConfigurationParameterType> &deviceConfigurationParameterTypes, std::vector<std::tuple<UwbDeviceConfigurationParameterType, UwbStatus, std::optional<UwbDeviceConfigurationParameter>>> &deviceConfigurationParameterResults) = 0;

    /**
     * @brief Set uwb device configuration parameters.
     *
     * @param deviceConfigurationParameters
     * @param deviceConfigurationParameterResults
     * @return UwbStatus
     */
    virtual UwbStatus
    DeviceSetConfigurationParameters(const std::vector<UwbDeviceConfigurationParameter> &deviceConfigurationParameters, std::vector<std::tuple<UwbDeviceConfigurationParameterType, UwbStatus>> &deviceConfigurationParameterResults) = 0;

    /**
     * @brief Get the session application configuration parameters.
     *
     * @param sessionId
     * @param applicationConfigurationParameterTypes
     * @param applicationConfigurationParameters
     * @return UwbStatus
     */
    virtual UwbStatus
    GetApplicationConfigurationParameters(uint32_t sessionId, std::vector<UwbApplicationConfigurationParameterType> applicationConfigurationParameterTypes, std::vector<UwbApplicationConfigurationParameter> &applicationConfigurationParameters) = 0;

    /**
     * @brief Set the session application configuration parameters.
     *
     * @param sessionId
     * @param uwbApplicationConfigurationParameters
     * @param uwbSetApplicationConfigurationParameterStatuses
     * @return UwbStatus
     */
    virtual UwbStatus
    SetApplicationConfigurationParameters(uint32_t sessionId, std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameter> &uwbApplicationConfigurationParameters, std::vector<::uwb::protocol::fira::UwbSetApplicationConfigurationParameterStatus> &uwbSetApplicationConfigurationParameterStatuses) = 0;

    /**
     * @brief Get the count of active sessions.
     *
     * @param sessionCount
     * @return UwbStatus
     */
    virtual UwbStatus
    GetSessionCount(uint32_t &sessionCount) = 0;

    /**
     * @brief Initialize a new session.
     *
     * @param sessionId
     * @param sessionType
     * @return UwbStatus
     */
    virtual UwbStatus
    SessionInitialize(uint32_t sessionId, UwbSessionType sessionType) = 0;

    /**
     * @brief Deinitialize an existing session.
     *
     * @param sessionId
     * @return UwbStatus
     */
    virtual UwbStatus
    SessionDeninitialize(uint32_t sessionId) = 0;

    /**
     * @brief Obtain the state of a session.
     *
     * @param sessionId
     * @param sessionState
     * @return UwbStatus
     */
    virtual UwbStatus
    SessionGetState(uint32_t sessionId, UwbSessionState &sessionState) = 0;

    /**
     * @brief Update the multicast list for a session.
     *
     * @param sessionId
     * @param action
     * @param updateMulticastListEntries
     * @return UwbStatus
     */
    virtual UwbStatus
    SessionUpdateControllerMulticastList(uint32_t sessionId, UwbMulticastAction action, std::vector<UwbSessionUpdateMulticastListEntry> updateMulticastListEntries) = 0;

    /**
     * @brief Start ranging in a session.
     *
     * @param sessionId
     * @return UwbStatus
     */
    virtual UwbStatus
    SessionStartRanging(uint32_t sessionId) = 0;

    /**
     * @brief Stop ranging in a session.
     *
     * @param sessionId
     * @return UwbStatus
     */
    virtual UwbStatus
    SessionStopRanging(uint32_t sessionId) = 0;

    /**
     * @brief Get the number of times ranging was performed in a session.
     *
     * @param sessionId
     * @param rangingCount
     * @return UwbStatus
     */
    virtual UwbStatus
    SessionGetRangingCount(uint32_t sessionId, uint32_t &rangingCount) = 0;

    /**
     * @brief Obtain uwb notification event data.
     *
     * @param notificationData
     * @return NTSTATUS
     */
    virtual NTSTATUS
    UwbNotification(UwbNotificationData &notificationData) = 0;
};
} // namespace windows::devices::uwb::simulator

#endif // I_UWB_SIMULATOR_DDI_CALLBACKS_LRP
