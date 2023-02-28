
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>

#include <windows.h>

#include "UwbSimulatorDdi.h"
#include "UwbSimulatorDdiCallbacksSimulatorImpl.hxx"
#include "UwbSimulatorDdiHandlerSimulator.hxx"

using namespace windows::devices::uwb::simulator;

/**
 * @brief Template function alias which partially specializes the
 * UwbSimulatorDispatchEntry template with ClassT = UwbSimulatorDdiHandlerLrp.
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
UwbSimulatorDispatchEntry<UwbSimulatorDdiHandlerSimulator> (*MakeLrpDispatchEntry)(ULONG, typename UwbSimulatorDispatchEntry<UwbSimulatorDdiHandlerSimulator>::HandlerFuncT) = &MakeDispatchEntry<UwbSimulatorDdiHandlerSimulator, InputT, OutputT>;

/**
 * @brief Dispatch table for the LRP DDI driver IOCTLs.
 *
 * This defines the expected input and output buffer sizes and the corresponding
 * member function that will handle the IOCTL.
 */
const std::initializer_list<UwbSimulatorDispatchEntry<UwbSimulatorDdiHandlerSimulator>> UwbSimulatorDdiHandlerSimulator::Dispatch{
    MakeLrpDispatchEntry<UwbSimulatorCapabilities, Unrestricted>(IOCTL_UWB_DEVICE_SIM_GET_CAPABILITIES, &UwbSimulatorDdiHandlerSimulator::OnUwbSimulatorCapabilities),
    MakeLrpDispatchEntry<UwbSimulatorTriggerSessionEventArgs, Unrestricted>(IOCTL_UWB_DEVICE_SIM_TRIGGER_SESSION_EVENT, &UwbSimulatorDdiHandlerSimulator::OnUwbSimulatorTriggerSessionEvent),
};

// IOCTL_UWB_DEVICE_RESET
NTSTATUS
UwbSimulatorDdiHandlerSimulator::OnUwbSimulatorCapabilities(WDFREQUEST /*request*/, std::span<uint8_t> /*inputBuffer*/, std::span<uint8_t> /*outputBuffer*/)
{
    NTSTATUS status = STATUS_SUCCESS;

    //// Execute callback.
    // auto statusUwb = m_callbacks->DeviceReset();

    //// Convert neutral types to DDI types.
    // auto &outputValue = *reinterpret_cast<UWB_STATUS *>(std::data(outputBuffer));
    // outputValue = UwbCxDdi::From(statusUwb);

    //// Complete the request.
    // WdfRequestCompleteWithInformation(request, status, sizeof outputValue);

    return status;
}

// IOCTL_UWB_GET_DEVICE_INFO
NTSTATUS
UwbSimulatorDdiHandlerSimulator::OnUwbSimulatorTriggerSessionEvent(WDFREQUEST /*request*/, std::span<uint8_t> /*inputBuffer*/, std::span<uint8_t> /*outputBuffer*/)
{
    NTSTATUS status = STATUS_SUCCESS;

    //// Execute callback.
    // UwbDeviceInformation deviceInformation{};
    // auto statusUwb = m_callbacks->DeviceGetInformation(deviceInformation);

    //// Convert neutral types to DDI types.
    // auto &outputValue = *reinterpret_cast<UWB_DEVICE_INFO *>(std::data(outputBuffer));
    // outputValue = UwbCxDdi::From(deviceInformation);

    //// Complete the request.
    // WdfRequestCompleteWithInformation(request, status, outputValue.size);

    return status;
}

UwbSimulatorDdiHandlerSimulator::UwbSimulatorDdiHandlerSimulator() :
    m_callbacks(std::make_unique<UwbSimulatorDdiCallbacksSimulatorImpl>())
{
}

std::optional<UwbSimulatorDispatchEntry<UwbSimulatorDdiHandlerSimulator>>
UwbSimulatorDdiHandlerSimulator::TryGetDispatchEntry(ULONG ioControlCode)
{
    const auto dispatchEntryIt = std::ranges::find_if(Dispatch, [&](const auto &dispatchEntry) {
        return (dispatchEntry.IoControlCode == ioControlCode);
    });

    return (dispatchEntryIt != std::end(Dispatch))
        ? *dispatchEntryIt
        : std::optional<UwbSimulatorDispatchEntry<UwbSimulatorDdiHandlerSimulator>>(std::nullopt);
}

bool
UwbSimulatorDdiHandlerSimulator::HandlesIoControlCode(ULONG ioControlCode)
{
    return std::ranges::any_of(Dispatch, [&](const auto &dispatchEntry) {
        return (dispatchEntry.IoControlCode == ioControlCode);
    });
}

NTSTATUS
UwbSimulatorDdiHandlerSimulator::ValidateRequest(WDFREQUEST /* request */, ULONG ioControlCode, std::size_t inputBufferLength, std::size_t outputBufferLength)
{
    const auto dispatchEntry = TryGetDispatchEntry(ioControlCode);
    NTSTATUS status = dispatchEntry.has_value()
        ? dispatchEntry->GetRequestValidityStatus(inputBufferLength, outputBufferLength)
        : STATUS_NOT_SUPPORTED;
    return status;
}

NTSTATUS
UwbSimulatorDdiHandlerSimulator::HandleRequest(WDFREQUEST request, ULONG ioControlCode, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer)
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
