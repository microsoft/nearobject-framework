
#include <algorithm>
#include <functional>
#include <ranges>
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
    MakeLrpDispatchEntry<Unrestricted, Unrestricted>(IOCTL_UWB_GET_DEVICE_INFO, &UwbSimulatorDdiHandler::OnUwbGetDeviceInformation),
    MakeLrpDispatchEntry<Unrestricted, Unrestricted>(IOCTL_UWB_GET_DEVICE_CAPABILITIES, &UwbSimulatorDdiHandler::OnUwbGetDeviceCapabilities),
    MakeLrpDispatchEntry<UWB_SET_DEVICE_CONFIG_PARAMS, Unrestricted>(IOCTL_UWB_GET_DEVICE_CONFIG_PARAMS, &UwbSimulatorDdiHandler::OnUwbGetDeviceConfigurationParameters),
    MakeLrpDispatchEntry<UWB_GET_DEVICE_CONFIG_PARAMS, Unrestricted>(IOCTL_UWB_SET_DEVICE_CONFIG_PARAMS, &UwbSimulatorDdiHandler::OnUwbSetDeviceConfigurationParameters),
    MakeLrpDispatchEntry<UWB_GET_APP_CONFIG_PARAMS, Unrestricted>(IOCTL_UWB_GET_APP_CONFIG_PARAMS, &UwbSimulatorDdiHandler::OnUwbGetApplicationConfigurationParameters),
    MakeLrpDispatchEntry<UWB_SET_APP_CONFIG_PARAMS, Unrestricted>(IOCTL_UWB_SET_APP_CONFIG_PARAMS, &UwbSimulatorDdiHandler::OnUwbSetApplicationConfigurationParameters),
    MakeLrpDispatchEntry<Unrestricted, UWB_GET_SESSION_COUNT>(IOCTL_UWB_GET_SESSION_COUNT, &UwbSimulatorDdiHandler::OnUwbGetSessionCount),
    MakeLrpDispatchEntry<UWB_SESSION_INIT, UWB_STATUS>(IOCTL_UWB_SESSION_INIT, &UwbSimulatorDdiHandler::OnUwbSessionInitialize),
    MakeLrpDispatchEntry<UWB_SESSION_DEINIT, UWB_STATUS>(IOCTL_UWB_SESSION_DEINIT, &UwbSimulatorDdiHandler::OnUwbSessionDeinitialize),
    MakeLrpDispatchEntry<UWB_GET_SESSION_STATE, UWB_SESSION_STATE_STATUS>(IOCTL_UWB_GET_SESSION_STATE, &UwbSimulatorDdiHandler::OnUwbGetSessionState),
    MakeLrpDispatchEntry<UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST, UWB_STATUS>(IOCTL_UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST, &UwbSimulatorDdiHandler::OnUwbSessionUpdateControllerMulticastList),
    MakeLrpDispatchEntry<UWB_START_RANGING_SESSION, UWB_STATUS>(IOCTL_UWB_START_RANGING_SESSION, &UwbSimulatorDdiHandler::OnUwbSessionStartRanging),
    MakeLrpDispatchEntry<UWB_STOP_RANGING_SESSION, UWB_STATUS>(IOCTL_UWB_STOP_RANGING_SESSION, &UwbSimulatorDdiHandler::OnUwbSessionStopRanging),
    MakeLrpDispatchEntry<UWB_GET_RANGING_COUNT, UWB_RANGING_COUNT>(IOCTL_UWB_GET_RANGING_COUNT, &UwbSimulatorDdiHandler::OnUwbSessionGetRangingCount),
    MakeLrpDispatchEntry<Unrestricted, Unrestricted>(IOCTL_UWB_NOTIFICATION, &UwbSimulatorDdiHandler::OnUwbNotification),
    // GUID_DEVINTERFACE_UWB_SIMULATOR Handlers
    MakeLrpDispatchEntry<Unrestricted, UwbSimulatorCapabilities>(IOCTL_UWB_DEVICE_SIM_GET_CAPABILITIES, &UwbSimulatorDdiHandler::OnUwbSimulatorCapabilities),
    MakeLrpDispatchEntry<UwbSimulatorTriggerSessionEventArgs, Unrestricted>(IOCTL_UWB_DEVICE_SIM_TRIGGER_SESSION_EVENT, &UwbSimulatorDdiHandler::OnUwbSimulatorTriggerSessionEvent),
};

UwbSimulatorDdiHandler::UwbSimulatorDdiHandler(UwbSimulatorDeviceFile *deviceFile) :
    m_deviceFile(deviceFile),
    m_callbacks(std::make_unique<UwbSimulatorDdiCallbacks>(deviceFile))
{
}

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
    std::size_t outputSize = 0;
    NTSTATUS status = STATUS_SUCCESS;

    // Execute callback.
    UwbDeviceInformation deviceInformation{};
    auto statusUwb = m_callbacks->DeviceGetInformation(deviceInformation);

    // Convert neutral type to DDI output type.
    auto uwbDeviceInformation = UwbCxDdi::From(deviceInformation);
    outputSize = std::size(uwbDeviceInformation);

    // Update output buffer if sufficiently sized.
    if (std::size(outputBuffer) >= outputSize) {
        std::memcpy(std::data(outputBuffer), std::data(std::data(uwbDeviceInformation)), outputSize);
    } else {
        status = STATUS_BUFFER_TOO_SMALL;
    }

    // Complete the request.
    WdfRequestCompleteWithInformation(request, status, outputSize);

    return status;
}

// IOCTL_UWB_GET_DEVICE_CAPABILITIES
NTSTATUS
UwbSimulatorDdiHandler::OnUwbGetDeviceCapabilities(WDFREQUEST request, std::span<uint8_t> /*inputBuffer*/, std::span<uint8_t> outputBuffer)
{
    std::size_t outputSize = 0;
    NTSTATUS status = STATUS_SUCCESS;

    // Execute callback.
    UwbCapability deviceCapabilities{};
    auto statusUwb = m_callbacks->DeviceGetCapabilities(deviceCapabilities);

    // Convert neutral type to DDI output type.
    auto uwbCapabilities = UwbCxDdi::From(deviceCapabilities);
    outputSize = std::size(uwbCapabilities);

    // Update output buffer if sufficiently sized.
    if (std::size(outputBuffer) >= outputSize) {
        std::memcpy(std::data(outputBuffer), std::data(std::data(uwbCapabilities)), outputSize);
    } else {
        status = STATUS_BUFFER_TOO_SMALL;
    }

    // Complete the request.
    WdfRequestCompleteWithInformation(request, status, outputSize);

    return status;
}

// IOCTL_UWB_GET_DEVICE_CONFIG_PARAMS
NTSTATUS
UwbSimulatorDdiHandler::OnUwbGetDeviceConfigurationParameters(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer)
{
    std::size_t outputSize = 0;
    NTSTATUS status = STATUS_SUCCESS;

    // Convert DDI input type to neutral type.
    auto &getDeviceConfigParamsIn = *reinterpret_cast<UWB_GET_DEVICE_CONFIG_PARAMS *>(std::data(inputBuffer));
    std::vector<UwbDeviceConfigurationParameterType> deviceConfigurationParameterTypes{};
    {
        std::span deviceConfigParamsRange(getDeviceConfigParamsIn.deviceConfigParams, getDeviceConfigParamsIn.deviceConfigParamsCount);
        std::ranges::transform(deviceConfigParamsRange, std::back_inserter(deviceConfigurationParameterTypes), [](const auto &deviceConfigParam) {
            return UwbCxDdi::To(deviceConfigParam);
        });
    }

    // Execute callback.
    std::vector<std::tuple<UwbDeviceConfigurationParameterType, UwbStatus, std::optional<UwbDeviceConfigurationParameter>>> deviceConfigurationParameterResults{};
    auto statusUwb = m_callbacks->DeviceGetConfigurationParameters(deviceConfigurationParameterTypes, deviceConfigurationParameterResults);

    // Convert neutral type to DDI output type.
    // TODO: auto deviceConfigParams = UwbCxDdi::From(deviceConfigurationParameterResults);
    // TODO: outputBufferSize = deviceConfigParams.size();

    // Update output buffer if sufficiently sized.
    if (std::size(outputBuffer) >= outputSize) {
        // TODO: std::memcpy(std::data(outputBuffer), std::data(std::data(<neutal wrapper>)), outputBufferSize);
    } else {
        status = STATUS_BUFFER_TOO_SMALL;
    }

    // Complete the request.
    WdfRequestCompleteWithInformation(request, status, outputSize);

    return status;
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
    auto &applicationConfigurationParametersIn = *reinterpret_cast<UWB_GET_APP_CONFIG_PARAMS *>(std::data(inputBuffer));

    std::vector<UwbApplicationConfigurationParameterType> uwbApplicationConfigurationParameterTypes{};
    uwbApplicationConfigurationParameterTypes.reserve(applicationConfigurationParametersIn.appConfigParamsCount);
    std::span uwbApplicationConfigurationParameterTypesRange{ applicationConfigurationParametersIn.appConfigParams, applicationConfigurationParametersIn.appConfigParamsCount };
    std::ranges::transform(uwbApplicationConfigurationParameterTypesRange, std::back_inserter(uwbApplicationConfigurationParameterTypes), [](const auto &applicationConfigurationParameterType) {
        return UwbCxDdi::To(applicationConfigurationParameterType);
    });

    // Invoke callback.
    std::vector<UwbApplicationConfigurationParameter> uwbApplicationConfigurationParameters{};
    auto statusUwb = m_callbacks->GetApplicationConfigurationParameters(applicationConfigurationParametersIn.sessionId, uwbApplicationConfigurationParameterTypes, uwbApplicationConfigurationParameters);

    // Convert neutral types to DDI types.
    auto &outputValue = *reinterpret_cast<UWB_APP_CONFIG_PARAMS *>(std::data(outputBuffer));
    if (IsUwbStatusOk(statusUwb)) {
        auto applicationConfigurationParametersWrapper = UwbCxDdi::From(uwbApplicationConfigurationParameters);
        auto applicationConfigurationParameterBuffer = std::data(applicationConfigurationParametersWrapper);
        std::ranges::copy(applicationConfigurationParameterBuffer, std::begin(outputBuffer));
    } else {
        outputValue.size = sizeof outputValue;
        outputValue.status = UwbCxDdi::From(statusUwb);
        outputValue.appConfigParamsCount = 0;
    }

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
    std::vector<UwbApplicationConfigurationParameter> applicationConfigurationParameters{};
    std::vector<UwbSetApplicationConfigurationParameterStatus> applicationConfigurationParameterResults{};

    // Invoke callback.
    auto statusUwb = m_callbacks->SetApplicationConfigurationParameters(applicationConfigurationParametersIn.sessionId, applicationConfigurationParameters, applicationConfigurationParameterResults);

    // Convert neutral type to DDI output type.
    auto &outputValue = *reinterpret_cast<UWB_SET_APP_CONFIG_PARAMS_STATUS *>(std::data(outputBuffer));
    outputValue.size = sizeof outputValue;
    outputValue.status = UwbCxDdi::From(statusUwb);

    // TODO: the output value needs to be filled in with applicationConfigurationParameterResults

    // Complete the request.
    WdfRequestCompleteWithInformation(request, status, outputValue.size);

    return status;
}

// IOCTL_UWB_GET_SESSION_COUNT
NTSTATUS
UwbSimulatorDdiHandler::OnUwbGetSessionCount(WDFREQUEST request, std::span<uint8_t> /*inputBuffer*/, std::span<uint8_t> outputBuffer)
{
    NTSTATUS status = STATUS_SUCCESS;

    // Invoke callback.
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
    auto sessionId = sessionInitIn.sessionId;

    // Invoke callback.
    auto statusUwb = m_callbacks->SessionInitialize(sessionId, sessionType);

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
    uint32_t sessionId = sessionDeinitIn.sessionId;

    // Invoke callback.
    auto statusUwb = m_callbacks->SessionDeninitialize(sessionId);

    // Convert neutral type to DDI output type.
    auto &outputValue = *reinterpret_cast<UWB_STATUS *>(std::data(outputBuffer));
    outputValue = UwbCxDdi::From(statusUwb);

    // Complete request.
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
    auto sessionId = sessionStateIn.sessionId;

    // Invoke callback.
    UwbSessionState sessionStateResult{};
    auto statusUwb = m_callbacks->SessionGetState(sessionId, sessionStateResult);

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
UwbSimulatorDdiHandler::OnUwbSessionUpdateControllerMulticastList(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer)
{
    NTSTATUS status = STATUS_SUCCESS;

    // Convert DDI input type to neutral types.
    auto &sessionUupdateControllerMuliticastListInRaw = *reinterpret_cast<UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST *>(std::data(inputBuffer));
    auto sessionUpdateControllerMuliticastListIn = UwbCxDdi::To(sessionUupdateControllerMuliticastListInRaw);
    auto sessionId = sessionUpdateControllerMuliticastListIn.SessionId;
    auto updateMulticastListEntries = std::move(sessionUpdateControllerMuliticastListIn.Controlees);
    auto action = sessionUpdateControllerMuliticastListIn.Action;

    // Invoke callback.
    auto statusUwb = m_callbacks->SessionUpdateControllerMulticastList(sessionId, action, std::move(updateMulticastListEntries));

    // Convert neutral types to DDI types.
    auto &outputValue = *reinterpret_cast<UWB_STATUS *>(std::data(outputBuffer));
    outputValue = UwbCxDdi::From(statusUwb);

    // Complete the request.
    WdfRequestCompleteWithInformation(request, status, sizeof outputValue);

    return status;
}

// IOCTL_UWB_START_RANGING_SESSION
NTSTATUS
UwbSimulatorDdiHandler::OnUwbSessionStartRanging(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer)
{
    NTSTATUS status = STATUS_SUCCESS;

    // Convert DDI input type to neutral type.
    auto &startRangingSessionIn = *reinterpret_cast<UWB_START_RANGING_SESSION *>(std::data(inputBuffer));
    auto sessionId = startRangingSessionIn.sessionId;

    // Invoke callback.
    auto statusUwb = m_callbacks->SessionStartRanging(sessionId);

    // Convert neutral types to DDI types.
    auto &outputValue = *reinterpret_cast<UWB_STATUS *>(std::data(outputBuffer));
    outputValue = UwbCxDdi::From(statusUwb);

    // Complete the request.
    WdfRequestCompleteWithInformation(request, status, sizeof outputValue);

    return status;
}

// IOCTL_UWB_STOP_RANGING_SESSION
NTSTATUS
UwbSimulatorDdiHandler::OnUwbSessionStopRanging(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer)
{
    NTSTATUS status = STATUS_SUCCESS;

    // Convert DDI input type to neutral type.
    auto &stopRangingSessionIn = *reinterpret_cast<UWB_STOP_RANGING_SESSION *>(std::data(inputBuffer));
    auto sessionId = stopRangingSessionIn.sessionId;

    // Invoke callback.
    auto statusUwb = m_callbacks->SessionStopRanging(sessionId);

    // Convert neutral types to DDI types.
    auto &outputValue = *reinterpret_cast<UWB_STATUS *>(std::data(outputBuffer));
    outputValue = UwbCxDdi::From(statusUwb);

    // Complete the request.
    WdfRequestCompleteWithInformation(request, status, sizeof outputValue);

    return status;
}

// IOCTL_UWB_GET_RANGING_COUNT
NTSTATUS
UwbSimulatorDdiHandler::OnUwbSessionGetRangingCount(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer)
{
    NTSTATUS status = STATUS_SUCCESS;

    // Convert DDI input type to neutral type.
    auto &getRangingCountIn = *reinterpret_cast<UWB_GET_RANGING_COUNT *>(std::data(inputBuffer));
    auto sessionId = getRangingCountIn.sessionId;

    // Invoke callback.
    uint32_t rangingCount = 0;
    auto statusUwb = m_callbacks->SessionGetRangingCount(sessionId, rangingCount);

    // Convert neutral types to DDI types.
    auto &outputValue = *reinterpret_cast<UWB_RANGING_COUNT *>(std::data(outputBuffer));
    outputValue.size = sizeof outputValue;
    outputValue.status = UwbCxDdi::From(statusUwb);
    outputValue.count = rangingCount;

    // Complete the request.
    WdfRequestCompleteWithInformation(request, status, outputValue.size);

    return status;
}

// IOCTL_UWB_NOTIFICATION
NTSTATUS
UwbSimulatorDdiHandler::OnUwbNotification(WDFREQUEST request, std::span<uint8_t> /*inputBuffer*/, std::span<uint8_t> outputBuffer)
{
    std::size_t outputBufferSize = std::size(outputBuffer);

    std::optional<UwbNotificationData> uwbNotificationData;
    auto *ioEventQueue = m_deviceFile->GetIoEventQueue();
    NTSTATUS status = ioEventQueue->HandleNotificationRequest(request, uwbNotificationData, outputBufferSize);
    if (status == STATUS_SUCCESS) {
        // Convert neutral type to DDI output type, and copy to output buffer.
        auto notificationData = UwbCxDdi::From(uwbNotificationData.value());
        std::memcpy(std::data(outputBuffer), std::data(std::data(notificationData)), outputBufferSize);
    }

    // Complete the request only if it has not been pended by the driver.
    if (status != STATUS_PENDING) {
        WdfRequestCompleteWithInformation(request, status, outputBufferSize);
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
UwbSimulatorDdiHandler::ValidateRequest(WDFREQUEST /*request*/, ULONG ioControlCode, std::size_t inputBufferLength, std::size_t outputBufferLength)
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
