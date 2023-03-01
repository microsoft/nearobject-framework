
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>

#include "UwbSimulatorDdiCallbacks.hxx"
#include "UwbSimulatorDdiHandler.hxx"

#include <windows/devices/uwb/UwbCxAdapterDdiLrp.hxx>
#include <windows/devices/uwb/UwbCxDdiLrp.hxx>

using namespace windows::devices::uwb::simulator;

/**
 * @brief Namespace alias to reduce typing but preserve clarity regarding DDI
 * conversion.
 */
namespace UwbCxDdi = windows::devices::uwb::ddi::lrp;

/**
 * @brief Template function alias which partially specializes the
 * UwbSimulatorDispatchEntry template with ClassT = UwbSimulatorDdiHandler.
 * This reduces some typing.
 *
 * It's not possible to create an alias to a templated function, however, it is
 * posssible to create an alias of a function pointer variable. Hence, this
 * template alias is defined with a pointer (`*MakeLrpDispatchEntry') and
 * assigned the address of MakeDispatchEntry.
 *
 * @tparam The type of the IOCTL input that's accepted.
 * @tparam The type of the IOCTL ouput that's accepted.
 */
template <
    typename InputT = Unrestricted,
    typename OutputT = Unrestricted>
UwbSimulatorDispatchEntry<UwbSimulatorDdiHandler> (*MakeLrpDispatchEntry)(ULONG, typename UwbSimulatorDispatchEntry<UwbSimulatorDdiHandler>::HandlerFuncT) = &MakeDispatchEntry<UwbSimulatorDdiHandler, InputT, OutputT>;

/**
 * @brief Dispatch table for the LRP DDI driver IOCTLs.
 *
 * This defines the expected input and output buffer sizes and the corresponding
 * member function that will handle the IOCTL.
 */
const std::initializer_list<UwbSimulatorDispatchEntry<UwbSimulatorDdiHandler>> UwbSimulatorDdiHandler::Dispatch{
    // GUID_UWB_DEVICE_INTERFACE Handlers
    MakeLrpDispatchEntry<UWB_DEVICE_RESET, UWB_STATUS>(IOCTL_UWB_DEVICE_RESET, &UwbSimulatorDdiHandler::OnUwbDeviceReset),
    MakeLrpDispatchEntry<Unrestricted, UWB_DEVICE_INFO>(IOCTL_UWB_GET_DEVICE_INFO, &UwbSimulatorDdiHandler::OnUwbGetDeviceInformation),
    MakeLrpDispatchEntry<Unrestricted, UWB_DEVICE_CAPABILITIES>(IOCTL_UWB_GET_DEVICE_CAPABILITIES, &UwbSimulatorDdiHandler::OnUwbGetDeviceCapabilities),
    MakeLrpDispatchEntry<UWB_SET_DEVICE_CONFIG_PARAMS, UWB_SET_DEVICE_CONFIG_PARAMS_STATUS>(IOCTL_UWB_GET_DEVICE_CONFIG_PARAMS, &UwbSimulatorDdiHandler::OnUwbGetDeviceConfigurationParameters),
    MakeLrpDispatchEntry<UWB_GET_DEVICE_CONFIG_PARAMS, UWB_DEVICE_CONFIG_PARAMS>(IOCTL_UWB_SET_DEVICE_CONFIG_PARAMS, &UwbSimulatorDdiHandler::OnUwbSetDeviceConfigurationParameters),
    MakeLrpDispatchEntry<UWB_GET_APP_CONFIG_PARAMS, UWB_APP_CONFIG_PARAMS>(IOCTL_UWB_GET_APP_CONFIG_PARAMS, &UwbSimulatorDdiHandler::OnUwbGetApplicationConfigurationParameters),
    MakeLrpDispatchEntry<UWB_SET_APP_CONFIG_PARAMS, UWB_SET_APP_CONFIG_PARAMS_STATUS>(IOCTL_UWB_SET_APP_CONFIG_PARAMS, &UwbSimulatorDdiHandler::OnUwbSetApplicationConfigurationParameters),
    MakeLrpDispatchEntry<Unrestricted, UWB_GET_SESSION_COUNT>(IOCTL_UWB_GET_SESSION_COUNT, &UwbSimulatorDdiHandler::OnUwbGetSessionCount),
    MakeLrpDispatchEntry<UWB_SESSION_INIT, UWB_STATUS>(IOCTL_UWB_SESSION_INIT, &UwbSimulatorDdiHandler::OnUwbSessionInitialize),
    MakeLrpDispatchEntry<UWB_SESSION_DEINIT, UWB_STATUS>(IOCTL_UWB_SESSION_DEINIT, &UwbSimulatorDdiHandler::OnUwbSessionDeinitialize),
    MakeLrpDispatchEntry<UWB_GET_SESSION_STATE, UWB_SESSION_STATE_STATUS>(IOCTL_UWB_GET_SESSION_STATE, &UwbSimulatorDdiHandler::OnUwbGetSessionState),
    MakeLrpDispatchEntry<UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST, UWB_STATUS>(IOCTL_UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST, &UwbSimulatorDdiHandler::OnUwbSessionUpdateControllerMulticastList),
    MakeLrpDispatchEntry<UWB_START_RANGING_SESSION, UWB_STATUS>(IOCTL_UWB_START_RANGING_SESSION, &UwbSimulatorDdiHandler::OnUwbSessionStartRanging),
    MakeLrpDispatchEntry<UWB_STOP_RANGING_SESSION, UWB_STATUS>(IOCTL_UWB_STOP_RANGING_SESSION, &UwbSimulatorDdiHandler::OnUwbSessionStopRanging),
    MakeLrpDispatchEntry<UWB_GET_RANGING_COUNT, UWB_RANGING_COUNT>(IOCTL_UWB_GET_RANGING_COUNT, &UwbSimulatorDdiHandler::OnUwbSessionGetRangingCount),
    MakeLrpDispatchEntry<Unrestricted, UWB_NOTIFICATION_DATA>(IOCTL_UWB_NOTIFICATION, &UwbSimulatorDdiHandler::OnUwbNotification),
    // GUID_DEVINTERFACE_UWB_SIMULATOR Handlers
    MakeLrpDispatchEntry<UwbSimulatorCapabilities, Unrestricted>(IOCTL_UWB_DEVICE_SIM_GET_CAPABILITIES, &UwbSimulatorDdiHandler::OnUwbSimulatorCapabilities),
    MakeLrpDispatchEntry<UwbSimulatorTriggerSessionEventArgs, Unrestricted>(IOCTL_UWB_DEVICE_SIM_TRIGGER_SESSION_EVENT, &UwbSimulatorDdiHandler::OnUwbSimulatorTriggerSessionEvent),
};

// IOCTL_UWB_DEVICE_RESET
NTSTATUS
UwbSimulatorDdiHandler::OnUwbDeviceReset(WDFREQUEST request, std::span<uint8_t> /*inputBuffer*/, std::span<uint8_t> outputBuffer)
{
    NTSTATUS status = STATUS_SUCCESS;

    // Execute callback.
    auto statusUwb = m_callbacks->DeviceReset();

    // Convert neutral types to DDI types.
    auto &outputValue = *reinterpret_cast<UWB_STATUS *>(std::data(outputBuffer));
    outputValue = UwbCxDdi::From(statusUwb);

    // Complete the request.
    WdfRequestCompleteWithInformation(request, status, sizeof outputValue);

    return status;
}

// IOCTL_UWB_GET_DEVICE_INFO
NTSTATUS
UwbSimulatorDdiHandler::OnUwbGetDeviceInformation(WDFREQUEST request, std::span<uint8_t> /*inputBuffer*/, std::span<uint8_t> outputBuffer)
{
    NTSTATUS status = STATUS_SUCCESS;

    // Execute callback.
    UwbDeviceInformation deviceInformation{};
    auto statusUwb = m_callbacks->DeviceGetInformation(deviceInformation);

    // Convert neutral types to DDI types.
    auto &outputValue = *reinterpret_cast<UWB_DEVICE_INFO *>(std::data(outputBuffer));
    outputValue = UwbCxDdi::From(deviceInformation);

    // Complete the request.
    WdfRequestCompleteWithInformation(request, status, outputValue.size);

    return status;
}

// IOCTL_UWB_GET_DEVICE_CAPABILITIES
NTSTATUS
UwbSimulatorDdiHandler::OnUwbGetDeviceCapabilities(WDFREQUEST request, std::span<uint8_t> /*inputBuffer*/, std::span<uint8_t> outputBuffer)
{
    NTSTATUS status = STATUS_SUCCESS;

    // Execute callback.
    UwbCapability deviceCapabilities{};
    auto statusUwb = m_callbacks->DeviceGetCapabilities(deviceCapabilities);

    // Convert neutral types to DDI types.
    auto &outputValue = *reinterpret_cast<UWB_DEVICE_CAPABILITIES *>(std::data(outputBuffer));
    outputValue = UwbCxDdi::From(deviceCapabilities);

    // Complete the request.
    WdfRequestCompleteWithInformation(request, status, outputValue.size);

    return status;
}

// IOCTL_UWB_GET_DEVICE_CONFIG_PARAMS
NTSTATUS
UwbSimulatorDdiHandler::OnUwbGetDeviceConfigurationParameters(WDFREQUEST /*request*/, std::span<uint8_t> /*inputBuffer*/, std::span<uint8_t> /*outputBuffer*/)
{
    return STATUS_NOT_IMPLEMENTED;
}

// IOCTL_UWB_SET_DEVICE_CONFIG_PARAMS
NTSTATUS
UwbSimulatorDdiHandler::OnUwbSetDeviceConfigurationParameters(WDFREQUEST /*request*/, std::span<uint8_t> /*inputBuffer*/, std::span<uint8_t> /*outputBuffer*/)
{
    return STATUS_NOT_IMPLEMENTED;
}

// IOCTL_UWB_GET_APP_CONFIG_PARAMS
NTSTATUS
UwbSimulatorDdiHandler::OnUwbGetApplicationConfigurationParameters(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer)
{
    NTSTATUS status = STATUS_SUCCESS;

    // Convert DDI input type to neutral type.
    auto &applicationConfigurationParametersIn = *reinterpret_cast<UWB_SET_APP_CONFIG_PARAMS *>(std::data(inputBuffer));
    std::vector<std::shared_ptr<IUwbAppConfigurationParameter>> applicationConfigurationParameters{};
    std::vector<std::tuple<UwbApplicationConfigurationParameterType, UwbStatus, std::shared_ptr<IUwbAppConfigurationParameter>>> applicationConfigurationParameterResults;

    // Invoke callback.
    auto statusUwb = m_callbacks->SetApplicationConfigurationParameters(applicationConfigurationParametersIn.sessionId, applicationConfigurationParameters, applicationConfigurationParameterResults);

    // Convert neutral types to DDI types.
    auto &outputValue = *reinterpret_cast<UWB_SET_APP_CONFIG_PARAMS_STATUS *>(std::data(outputBuffer));
    outputValue.size = sizeof outputValue;
    outputValue.status = UwbCxDdi::From(statusUwb);

    // Complete the request.
    WdfRequestCompleteWithInformation(request, status, outputValue.size);

    return status;
}

// IOCTL_UWB_SET_APP_CONFIG_PARAMS
NTSTATUS
UwbSimulatorDdiHandler::OnUwbSetApplicationConfigurationParameters(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer)
{
    NTSTATUS status = STATUS_SUCCESS;

    // Convert DDI input type to neutral type.
    auto &applicationConfigurationParametersIn = *reinterpret_cast<UWB_SET_APP_CONFIG_PARAMS *>(std::data(inputBuffer));
    std::vector<std::shared_ptr<IUwbAppConfigurationParameter>> applicationConfigurationParameters{};
    std::vector<std::tuple<UwbApplicationConfigurationParameterType, UwbStatus, std::shared_ptr<IUwbAppConfigurationParameter>>> applicationConfigurationParameterResults;

    // Invoke callback.
    auto statusUwb = m_callbacks->SetApplicationConfigurationParameters(applicationConfigurationParametersIn.sessionId, applicationConfigurationParameters, applicationConfigurationParameterResults);

    // Convert neutral type to DDI output type.
    auto &outputValue = *reinterpret_cast<UWB_SET_APP_CONFIG_PARAMS_STATUS *>(std::data(outputBuffer));
    outputValue.size = sizeof outputValue;
    outputValue.status = UwbCxDdi::From(statusUwb);

    // Complete the request.
    WdfRequestCompleteWithInformation(request, status, outputValue.size);

    return status;
}

// IOCTL_UWB_GET_SESSION_COUNT
NTSTATUS
UwbSimulatorDdiHandler::OnUwbGetSessionCount(WDFREQUEST request, std::span<uint8_t> /*inputBuffer*/, std::span<uint8_t> outputBuffer)
{
    NTSTATUS status = STATUS_SUCCESS;

    // Invvoke callback.
    uint32_t sessionCount = 0;
    auto statusUwb = m_callbacks->GetSessionCount(sessionCount);
    
    // Convert neutral type to DDI output type.
    auto &outputValue = *reinterpret_cast<UWB_GET_SESSION_COUNT *>(std::data(outputBuffer));
    outputValue.size = sizeof outputValue;
    outputValue.status = UwbCxDdi::From(statusUwb);
    outputValue.sessionCount = sessionCount;

    // Complete the request.
    WdfRequestCompleteWithInformation(request, status, outputValue.size);

    return status;
}

// IOCTL_UWB_SESSION_INIT
NTSTATUS
UwbSimulatorDdiHandler::OnUwbSessionInitialize(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer)
{
    NTSTATUS status = STATUS_SUCCESS;

    // Convert DDI input type to neutral type.
    auto &sessionInitIn = *reinterpret_cast<UWB_SESSION_INIT *>(std::data(inputBuffer));
    auto sessionType = UwbCxDdi::To(sessionInitIn.sessionType);

    // Invoke callback.
    auto statusUwb = m_callbacks->SessionInitialize(sessionInitIn.sessionId, sessionType);

    // Convert neutral type to DDI output type.
    auto &outputValue = *reinterpret_cast<UWB_STATUS *>(std::data(outputBuffer));
    outputValue = UwbCxDdi::From(statusUwb);

    WdfRequestCompleteWithInformation(request, status, sizeof outputValue);

    return status;
}

// IOCTL_UWB_SESSION_DEINIT
NTSTATUS
UwbSimulatorDdiHandler::OnUwbSessionDeinitialize(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer)
{
    NTSTATUS status = STATUS_SUCCESS;

    // Convert DDI input type to neutral type.
    auto &sessionDeinitIn = *reinterpret_cast<UWB_SESSION_DEINIT *>(std::data(inputBuffer));

    // Invoke callback.
    auto statusUwb = m_callbacks->SessionDeninitialize(sessionDeinitIn.sessionId);

    // Convert neutral type to DDI output type.
    auto &outputValue = *reinterpret_cast<UWB_STATUS *>(std::data(outputBuffer));
    outputValue = UwbCxDdi::From(statusUwb);

    WdfRequestCompleteWithInformation(request, status, sizeof outputValue);

    return status;
}

// IOCTL_UWB_GET_SESSION_STATE
NTSTATUS
UwbSimulatorDdiHandler::OnUwbGetSessionState(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer)
{
    NTSTATUS status = STATUS_SUCCESS;

    // Convert DDI input type to neutral type.
    auto &sessionStateIn = *reinterpret_cast<UWB_GET_SESSION_STATE *>(std::data(inputBuffer));
    UwbSessionState sessionStateResult{};
    auto statusUwb = m_callbacks->SessionGetState(sessionStateIn.sessionId, sessionStateResult);

    // Convert neutral types to DDI types.
    auto &outputValue = *reinterpret_cast<UWB_SESSION_STATE_STATUS *>(std::data(outputBuffer));
    outputValue.size = sizeof outputValue;
    outputValue.status = UwbCxDdi::From(statusUwb);
    outputValue.sessionState = UwbCxDdi::From(sessionStateResult);

    // Complete the request.
    WdfRequestCompleteWithInformation(request, status, outputValue.size);

    return status;
}

// IOCTL_UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST
NTSTATUS
UwbSimulatorDdiHandler::OnUwbSessionUpdateControllerMulticastList(WDFREQUEST /*request*/, std::span<uint8_t> /*inputBuffer*/, std::span<uint8_t> /*outputBuffer*/)
{
    return STATUS_NOT_IMPLEMENTED;
}

// IOCTL_UWB_START_RANGING_SESSION
NTSTATUS
UwbSimulatorDdiHandler::OnUwbSessionStartRanging(WDFREQUEST /*request*/, std::span<uint8_t> /*inputBuffer*/, std::span<uint8_t> /*outputBuffer*/)
{
    return STATUS_NOT_IMPLEMENTED;
}

// IOCTL_UWB_STOP_RANGING_SESSION
NTSTATUS
UwbSimulatorDdiHandler::OnUwbSessionStopRanging(WDFREQUEST /*request*/, std::span<uint8_t> /*inputBuffer*/, std::span<uint8_t> /*outputBuffer*/)
{
    return STATUS_NOT_IMPLEMENTED;
}

// IOCTL_UWB_GET_RANGING_COUNT
NTSTATUS
UwbSimulatorDdiHandler::OnUwbSessionGetRangingCount(WDFREQUEST /*request*/, std::span<uint8_t> /*inputBuffer*/, std::span<uint8_t> /*outputBuffer*/)
{
    return STATUS_NOT_IMPLEMENTED;
}

// IOCTL_UWB_NOTIFICATION
NTSTATUS
UwbSimulatorDdiHandler::OnUwbNotification(WDFREQUEST request, std::span<uint8_t> /*inputBuffer*/, std::span<uint8_t> outputBuffer)
{
    UwbNotificationData uwbNotificationData{};
    NTSTATUS status = m_callbacks->UwbNotification(uwbNotificationData);

    if (status == STATUS_SUCCESS) {
        // Convert neutral types to DDI types.
        auto &outputValue = *reinterpret_cast<UWB_NOTIFICATION_DATA *>(std::data(outputBuffer));
        outputValue = UwbCxDdi::From(uwbNotificationData);

        // Complete the request.
        WdfRequestCompleteWithInformation(request, status, outputValue.size);
    } else {
        WdfRequestComplete(request, status);
    }

    return status;
}

// IOCTL_UWB_DEVICE_SIM_GET_CAPABILITIES
NTSTATUS
UwbSimulatorDdiHandler::OnUwbSimulatorCapabilities(WDFREQUEST request, std::span<uint8_t> /*inputBuffer*/, std::span<uint8_t> outputBuffer)
{
    NTSTATUS status = STATUS_SUCCESS;

    // Execute callback.
    auto uwbSimulatorCapabilities = m_callbacks->GetSimulatorCapabilities();

    // Copy output value to output buffer.
    auto &outputValue = *reinterpret_cast<decltype(uwbSimulatorCapabilities) *>(std::data(outputBuffer));
    outputValue = uwbSimulatorCapabilities;

    // Complete the request.
    WdfRequestCompleteWithInformation(request, status, sizeof outputValue);

    return status;
}

// IOCTL_UWB_DEVICE_SIM_TRIGGER_SESSION_EVENT
NTSTATUS
UwbSimulatorDdiHandler::OnUwbSimulatorTriggerSessionEvent(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> /*outputBuffer*/)
{
    NTSTATUS status = STATUS_SUCCESS;

    auto &triggerSessionEventArgs = *reinterpret_cast<UwbSimulatorTriggerSessionEventArgs *>(std::data(inputBuffer));

    // Execute callback.
    m_callbacks->TriggerSessionEvent(triggerSessionEventArgs);

    // Complete the request.
    WdfRequestComplete(request, STATUS_SUCCESS);

    return status;
}

UwbSimulatorDdiHandler::UwbSimulatorDdiHandler() :
    m_callbacks(std::make_unique<UwbSimulatorDdiCallbacks>())
{
}

std::optional<UwbSimulatorDispatchEntry<UwbSimulatorDdiHandler>>
UwbSimulatorDdiHandler::TryGetDispatchEntry(ULONG ioControlCode)
{
    const auto dispatchEntryIt = std::ranges::find_if(Dispatch, [&](const auto &dispatchEntry) {
        return (dispatchEntry.IoControlCode == ioControlCode);
    });

    return (dispatchEntryIt != std::end(Dispatch))
        ? *dispatchEntryIt
        : std::optional<UwbSimulatorDispatchEntry<UwbSimulatorDdiHandler>>(std::nullopt);
}

bool
UwbSimulatorDdiHandler::HandlesIoControlCode(ULONG ioControlCode)
{
    return std::ranges::any_of(Dispatch, [&](const auto &dispatchEntry) {
        return (dispatchEntry.IoControlCode == ioControlCode);
    });
}

NTSTATUS
UwbSimulatorDdiHandler::ValidateRequest(WDFREQUEST /* request */, ULONG ioControlCode, std::size_t inputBufferLength, std::size_t outputBufferLength)
{
    const auto dispatchEntry = TryGetDispatchEntry(ioControlCode);
    NTSTATUS status = dispatchEntry.has_value()
        ? dispatchEntry->GetRequestValidityStatus(inputBufferLength, outputBufferLength)
        : STATUS_NOT_SUPPORTED;
    return status;
}

NTSTATUS
UwbSimulatorDdiHandler::HandleRequest(WDFREQUEST request, ULONG ioControlCode, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer)
{
    auto dispatchEntry = TryGetDispatchEntry(ioControlCode);
    if (!dispatchEntry.has_value()) {
        return STATUS_NOT_SUPPORTED;
    } else if (dispatchEntry->Handler == nullptr) {
        return STATUS_NOT_IMPLEMENTED;
    }

    // The handler function is an unbound member function so use std::invoke to
    // bind it to this object instance, forwarding the request arguments.
    NTSTATUS status = std::invoke(dispatchEntry->Handler, this, request, inputBuffer, outputBuffer);
    return status;
}
