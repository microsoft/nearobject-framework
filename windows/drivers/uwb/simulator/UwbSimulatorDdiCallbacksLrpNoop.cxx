
#include "UwbSimulatorDdiCallbacksLrpNoop.hxx"

using namespace windows::devices::uwb;
using namespace windows::devices::uwb::simulator;

UwbStatus
UwbSimulatorDdiCallbacksLrpNoop::DeviceReset()
{
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacksLrpNoop::DeviceGetInformation(UwbDeviceInfo &deviceInfo)
{
    deviceInfo = {};
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacksLrpNoop::DeviceGetCapabilities(UwbDeviceCapabilities &deviceCapabilities)
{
    deviceCapabilities = {};
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacksLrpNoop::DeviceGetConfigurationParameters(std::vector<UwbDeviceConfigurationParameterType> & /* deviceConfigurationParameterTypes */, std::vector<std::tuple<UwbDeviceConfigurationParameterType, UwbStatus, std::optional<UwbDeviceConfigurationParameter>>> &deviceConfigurationParameterResults)
{
    deviceConfigurationParameterResults = {};
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacksLrpNoop::DeviceSetConfigurationParameters(const std::vector<UwbDeviceConfigurationParameter> & /* deviceConfigurationParameters */, std::vector<std::tuple<UwbDeviceConfigurationParameterType, UwbStatus>> &deviceConfigurationParameterResults)
{
    deviceConfigurationParameterResults = {};
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacksLrpNoop::SessionInitialize(uint32_t /*sessionId*/, UwbSessionType /*sessionType*/)
{
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacksLrpNoop::SessionDeninitialize(uint32_t /*sessionId*/)
{
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacksLrpNoop::SetApplicationConfigurationParameters(const std::vector<std::unique_ptr<IUwbAppConfigurationParameter>> & /* applicationConfigurationParameters */, std::vector<std::tuple<UwbApplicationConfigurationParameterType, UwbStatus, std::unique_ptr<IUwbAppConfigurationParameter>>> &applicationConfigurationParameterResults)
{
    std::vector<std::tuple<UwbApplicationConfigurationParameterType, UwbStatus, std::unique_ptr<IUwbAppConfigurationParameter>>> results{};
    applicationConfigurationParameterResults = std::move(results);
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacksLrpNoop::GetApplicationConfigurationParameters(std::vector<UwbApplicationConfigurationParameter> &applicationConfigurationParameters)
{
    applicationConfigurationParameters = {};
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacksLrpNoop::GetSessionCount(uint32_t * /* sessionCount */)
{
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacksLrpNoop::SessionGetState(uint32_t /* sessionId */, UwbSessionState * /* sessionState */)
{
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacksLrpNoop::SessionUpdateControllerMulticastList(const std::vector<UwbMacAddress> & /* controlees */)
{
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacksLrpNoop::SessionStartRanging(uint32_t /* sessionId */)
{
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacksLrpNoop::SessionStopRanging(uint32_t /* sessionId */)
{
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacksLrpNoop::SessionGetRangingCount(uint32_t /* sessionId */, uint32_t * /* rangingCount */)
{
    return UwbStatusOk;
}

void
UwbSimulatorDdiCallbacksLrpNoop::UwbNotification(UwbNotificationData notificationData)
{

}
