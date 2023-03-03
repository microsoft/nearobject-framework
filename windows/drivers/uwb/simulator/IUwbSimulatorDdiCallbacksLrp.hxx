
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
#include <windows/devices/uwb/UwbAppConfiguration.hxx>

#include <uwb/protocols/fira/FiraDevice.hxx>
#include <uwb/protocols/fira/UwbApplicationConfiguration.hxx>
#include <uwb/protocols/fira/UwbCapability.hxx>

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
     * @brief
     *
     * @return UwbStatus
     */
    virtual UwbStatus
    DeviceReset() = 0;

    /**
     * @brief
     *
     * @param deviceInfo
     * @return UwbStatus
     */
    virtual UwbStatus
    DeviceGetInformation(UwbDeviceInformation &deviceInfo) = 0;

    /**
     * @brief
     *
     * @param deviceCapabilities
     * @return UwbStatus
     */
    virtual UwbStatus
    DeviceGetCapabilities(UwbCapability &deviceCapabilities) = 0;

    /**
     * @brief
     *
     * @param deviceConfigurationParameterTypes
     * @param deviceConfigurationParameterResults
     * @return UwbStatus
     */
    virtual UwbStatus
    DeviceGetConfigurationParameters(std::vector<UwbDeviceConfigurationParameterType> &deviceConfigurationParameterTypes, std::vector<std::tuple<UwbDeviceConfigurationParameterType, UwbStatus, std::optional<UwbDeviceConfigurationParameter>>> &deviceConfigurationParameterResults) = 0;

    /**
     * @brief
     *
     * @param deviceConfigurationParameters
     * @param deviceConfigurationParameterResults
     * @return UwbStatus
     *
     * TODO: possibly separate the results into a vector with ones that had UwbStatus::Ok, and a separate vector with the ones that weren't retrieved.
     */
    virtual UwbStatus
    DeviceSetConfigurationParameters(const std::vector<UwbDeviceConfigurationParameter> &deviceConfigurationParameters, std::vector<std::tuple<UwbDeviceConfigurationParameterType, UwbStatus>> &deviceConfigurationParameterResults) = 0;

    /**
     * @brief Get the Application Configuration Parameters object
     * 
     * @param sessionId 
     * @param applicationConfigurationParameterTypes 
     * @param applicationConfigurationParameters 
     * @return UwbStatus 
     */
    virtual UwbStatus
    GetApplicationConfigurationParameters(uint32_t sessionId, std::vector<UwbApplicationConfigurationParameterType> applicationConfigurationParameterTypes, std::vector<std::shared_ptr<IUwbAppConfigurationParameter>> &applicationConfigurationParameters) = 0;

    /**
     * @brief Set the Application Configuration Parameters object
     *
     * @param applicationConfigurationParameters
     * @param applicationConfigurationParameterResults
     * @return UwbStatus
     */
    virtual UwbStatus
    SetApplicationConfigurationParameters(uint32_t sessionId, const std::vector<std::shared_ptr<IUwbAppConfigurationParameter>> &applicationConfigurationParameters, std::vector<std::tuple<UwbApplicationConfigurationParameterType, UwbStatus, std::shared_ptr<IUwbAppConfigurationParameter>>> &applicationConfigurationParameterResults) = 0;

    /**
     * @brief Get the Session Count object
     *
     * @param sessionCount
     * @return UwbStatus
     */
    virtual UwbStatus
    GetSessionCount(uint32_t &sessionCount) = 0;

    /**
     * @brief
     *
     * @param sessionId
     * @param sessionType
     * @return UwbStatus
     */
    virtual UwbStatus
    SessionInitialize(uint32_t sessionId, UwbSessionType sessionType) = 0;

    /**
     * @brief
     *
     * @param sessionId
     * @return UwbStatus
     */
    virtual UwbStatus
    SessionDeninitialize(uint32_t sessionId) = 0;

    /**
     * @brief
     *
     * @param sessionId
     * @param sessionState
     * @return UwbStatus
     */
    virtual UwbStatus
    SessionGetState(uint32_t sessionId, UwbSessionState &sessionState) = 0;

    /**
     * @brief
     *
     * @param sessionId
     * @param action
     * @param updateMulticastListEntries
     * @return UwbStatus
     */
    virtual UwbStatus
    SessionUpdateControllerMulticastList(uint32_t sessionId, UwbMulticastAction action, std::vector<UwbSessionUpdateMulticastListEntry> updateMulticastListEntries) = 0;

    /**
     * @brief
     *
     * @param sessionId
     * @return UwbStatus
     */
    virtual UwbStatus
    SessionStartRanging(uint32_t sessionId) = 0;

    /**
     * @brief
     *
     * @param sessionId
     * @return UwbStatus
     */
    virtual UwbStatus
    SessionStopRanging(uint32_t sessionId) = 0;

    /**
     * @brief
     *
     * @param sessionId
     * @param rangingCount
     * @return UwbStatus
     */
    virtual UwbStatus
    SessionGetRangingCount(uint32_t sessionId, uint32_t &rangingCount) = 0;

    /**
     * @brief
     *
     * @param notificationData
     */
    virtual NTSTATUS
    UwbNotification(UwbNotificationData &notificationData) = 0;
};
} // namespace windows::devices::uwb::simulator

#endif // I_UWB_SIMULATOR_DDI_CALLBACKS_LRP
