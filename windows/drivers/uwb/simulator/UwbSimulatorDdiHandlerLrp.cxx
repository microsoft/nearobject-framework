
#include <algorithm>
#include <functional>

#include "UwbCxLrpDeviceGlue.h"

#include "UwbSimulatorDdiCallbacksLrpNoop.hxx"
#include "UwbSimulatorDdiHandlerLrp.hxx"

using namespace windows::devices::uwb::simulator;

/**
 * TODO: min+max sizes need filling in. Get these numbers from the UwbCx driver.
 */
const std::initializer_list<UwbSimulatorDispatchEntry<UwbSimulatorDdiHandlerLrp>> UwbSimulatorDdiHandlerLrp::Dispatch{
    UwbSimulatorDispatchEntry<UwbSimulatorDdiHandlerLrp>{ IOCTL_UWB_DEVICE_RESET, 0, 0, 0, 0, &UwbSimulatorDdiHandlerLrp::OnUwbDeviceReset },
    UwbSimulatorDispatchEntry<UwbSimulatorDdiHandlerLrp>{ IOCTL_UWB_GET_DEVICE_INFO, 0, 0, 0, 0, &UwbSimulatorDdiHandlerLrp::OnUwbGetDeviceInformation },
    UwbSimulatorDispatchEntry<UwbSimulatorDdiHandlerLrp>{ IOCTL_UWB_GET_DEVICE_CAPABILITIES, 0, 0, 0, 0, &UwbSimulatorDdiHandlerLrp::OnUwbGetDeviceCapabilities },
    UwbSimulatorDispatchEntry<UwbSimulatorDdiHandlerLrp>{ IOCTL_UWB_GET_DEVICE_CONFIG_PARAMS, 0, 0, 0, 0, &UwbSimulatorDdiHandlerLrp::OnUwbGetDeviceConfigurationParameters },
    UwbSimulatorDispatchEntry<UwbSimulatorDdiHandlerLrp>{ IOCTL_UWB_SET_DEVICE_CONFIG_PARAMS, 0, 0, 0, 0, &UwbSimulatorDdiHandlerLrp::OnUwbSetDeviceConfigurationParameters },
    UwbSimulatorDispatchEntry<UwbSimulatorDdiHandlerLrp>{ IOCTL_UWB_GET_APP_CONFIG_PARAMS, 0, 0, 0, 0, &UwbSimulatorDdiHandlerLrp::OnUwbGetApplicationConfigurationParameters },
    UwbSimulatorDispatchEntry<UwbSimulatorDdiHandlerLrp>{ IOCTL_UWB_SET_APP_CONFIG_PARAMS, 0, 0, 0, 0, &UwbSimulatorDdiHandlerLrp::OnUwbSetApplicationConfigurationParameters },
    UwbSimulatorDispatchEntry<UwbSimulatorDdiHandlerLrp>{ IOCTL_UWB_GET_SESSION_COUNT, 0, 0, 0, 0, &UwbSimulatorDdiHandlerLrp::OnUwbGetSessionCount },
    UwbSimulatorDispatchEntry<UwbSimulatorDdiHandlerLrp>{ IOCTL_UWB_SESSION_INIT, 0, 0, 0, 0, &UwbSimulatorDdiHandlerLrp::OnUwbSessionInitialize },
    UwbSimulatorDispatchEntry<UwbSimulatorDdiHandlerLrp>{ IOCTL_UWB_SESSION_DEINIT, 0, 0, 0, 0, &UwbSimulatorDdiHandlerLrp::OnUwbSessionDeinitialize },
    UwbSimulatorDispatchEntry<UwbSimulatorDdiHandlerLrp>{ IOCTL_UWB_GET_SESSION_STATE, 0, 0, 0, 0, &UwbSimulatorDdiHandlerLrp::OnUwbGetSessionState },
    UwbSimulatorDispatchEntry<UwbSimulatorDdiHandlerLrp>{ IOCTL_UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST, 0, 0, 0, 0, &UwbSimulatorDdiHandlerLrp::OnUwbSessionUpdateControllerMulticastList },
    UwbSimulatorDispatchEntry<UwbSimulatorDdiHandlerLrp>{ IOCTL_UWB_START_RANGING_SESSION, 0, 0, 0, 0, &UwbSimulatorDdiHandlerLrp::OnUwbSessionStartRanging },
    UwbSimulatorDispatchEntry<UwbSimulatorDdiHandlerLrp>{ IOCTL_UWB_STOP_RANGING_SESSION, 0, 0, 0, 0, &UwbSimulatorDdiHandlerLrp::OnUwbSessionStopRanging },
    UwbSimulatorDispatchEntry<UwbSimulatorDdiHandlerLrp>{ IOCTL_UWB_GET_RANGING_COUNT, 0, 0, 0, 0, &UwbSimulatorDdiHandlerLrp::OnUwbSessionGetRangingCount },
    UwbSimulatorDispatchEntry<UwbSimulatorDdiHandlerLrp>{ IOCTL_UWB_NOTIFICATION, 0, 0, 0, 0, &UwbSimulatorDdiHandlerLrp::OnUwbNotification },
};

namespace UwbCxDdi
{
/**
 * @brief
 * TODO: move this to its own file
 */
UWB_STATUS
From(const UwbStatus & /*uwbStatus*/)
{
    // TODO: implement this
    return UWB_STATUS_OK;
}
} // namespace UwbCxDdi

NTSTATUS
UwbSimulatorDdiHandlerLrp::OnUwbDeviceReset(WDFREQUEST request, std::span<uint8_t> /*inputBuffer*/, std::span<uint8_t> outputBuffer)
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

NTSTATUS
UwbSimulatorDdiHandlerLrp::OnUwbGetDeviceInformation(WDFREQUEST /*request*/, std::span<uint8_t> /*inputBuffer*/, std::span<uint8_t> /*outputBuffer*/)
{
    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS
UwbSimulatorDdiHandlerLrp::OnUwbGetDeviceCapabilities(WDFREQUEST /*request*/, std::span<uint8_t> /*inputBuffer*/, std::span<uint8_t> /*outputBuffer*/)
{
    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS
UwbSimulatorDdiHandlerLrp::OnUwbGetDeviceConfigurationParameters(WDFREQUEST /*request*/, std::span<uint8_t> /*inputBuffer*/, std::span<uint8_t> /*outputBuffer*/)
{
    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS
UwbSimulatorDdiHandlerLrp::OnUwbSetDeviceConfigurationParameters(WDFREQUEST /*request*/, std::span<uint8_t> /*inputBuffer*/, std::span<uint8_t> /*outputBuffer*/)
{
    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS
UwbSimulatorDdiHandlerLrp::OnUwbGetApplicationConfigurationParameters(WDFREQUEST /*request*/, std::span<uint8_t> /*inputBuffer*/, std::span<uint8_t> /*outputBuffer*/)
{
    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS
UwbSimulatorDdiHandlerLrp::OnUwbSetApplicationConfigurationParameters(WDFREQUEST /*request*/, std::span<uint8_t> /*inputBuffer*/, std::span<uint8_t> /*outputBuffer*/)
{
    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS
UwbSimulatorDdiHandlerLrp::OnUwbGetSessionCount(WDFREQUEST /*request*/, std::span<uint8_t> /*inputBuffer*/, std::span<uint8_t> /*outputBuffer*/)
{
    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS
UwbSimulatorDdiHandlerLrp::OnUwbSessionInitialize(WDFREQUEST /*request*/, std::span<uint8_t> /*inputBuffer*/, std::span<uint8_t> /*outputBuffer*/)
{
    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS
UwbSimulatorDdiHandlerLrp::OnUwbSessionDeinitialize(WDFREQUEST /*request*/, std::span<uint8_t> /*inputBuffer*/, std::span<uint8_t> /*outputBuffer*/)
{
    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS
UwbSimulatorDdiHandlerLrp::OnUwbGetSessionState(WDFREQUEST /*request*/, std::span<uint8_t> /*inputBuffer*/, std::span<uint8_t> /*outputBuffer*/)
{
    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS
UwbSimulatorDdiHandlerLrp::OnUwbSessionUpdateControllerMulticastList(WDFREQUEST /*request*/, std::span<uint8_t> /*inputBuffer*/, std::span<uint8_t> /*outputBuffer*/)
{
    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS
UwbSimulatorDdiHandlerLrp::OnUwbSessionStartRanging(WDFREQUEST /*request*/, std::span<uint8_t> /*inputBuffer*/, std::span<uint8_t> /*outputBuffer*/)
{
    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS
UwbSimulatorDdiHandlerLrp::OnUwbSessionStopRanging(WDFREQUEST /*request*/, std::span<uint8_t> /*inputBuffer*/, std::span<uint8_t> /*outputBuffer*/)
{
    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS
UwbSimulatorDdiHandlerLrp::OnUwbSessionGetRangingCount(WDFREQUEST /*request*/, std::span<uint8_t> /*inputBuffer*/, std::span<uint8_t> /*outputBuffer*/)
{
    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS
UwbSimulatorDdiHandlerLrp::OnUwbNotification(WDFREQUEST /*request*/, std::span<uint8_t> /*inputBuffer*/, std::span<uint8_t> /*outputBuffer*/)
{
    return STATUS_NOT_IMPLEMENTED;
}

UwbSimulatorDdiHandlerLrp::UwbSimulatorDdiHandlerLrp() :
    m_callbacks(std::make_unique<UwbSimulatorDdiCallbacksLrpNoop>())
{
}

std::optional<UwbSimulatorDispatchEntry<UwbSimulatorDdiHandlerLrp>>
UwbSimulatorDdiHandlerLrp::TryGetDispatchEntry(ULONG ioControlCode)
{
    const auto dispatchEntryIt = std::ranges::find_if(Dispatch, [&](const auto &dispatchEntry) {
        return (dispatchEntry.IoControlCode == ioControlCode);
    });

    return (dispatchEntryIt != std::end(Dispatch))
        ? *dispatchEntryIt
        : std::optional<UwbSimulatorDispatchEntry<UwbSimulatorDdiHandlerLrp>>(std::nullopt);
}

bool
UwbSimulatorDdiHandlerLrp::HandlesIoControlCode(ULONG ioControlCode)
{
    return std::ranges::any_of(Dispatch, [&](const auto &dispatchEntry) {
        return (dispatchEntry.IoControlCode == ioControlCode);
    });
}

NTSTATUS
UwbSimulatorDdiHandlerLrp::ValidateRequest(WDFREQUEST /* request */, ULONG ioControlCode, std::size_t inputBufferLength, std::size_t outputBufferLength)
{
    const auto dispatchEntry = TryGetDispatchEntry(ioControlCode);
    NTSTATUS status = dispatchEntry.has_value()
        ? dispatchEntry->GetRequestValidityStatus(inputBufferLength, outputBufferLength)
        : STATUS_NOT_SUPPORTED;
    return status;
}

NTSTATUS
UwbSimulatorDdiHandlerLrp::HandleRequest(WDFREQUEST request, ULONG ioControlCode, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer)
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
