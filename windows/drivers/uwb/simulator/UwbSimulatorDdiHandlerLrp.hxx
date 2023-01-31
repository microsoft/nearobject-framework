
#ifndef UWB_SIMULATOR_DDI_HANDLER_LRP_HXX
#define UWB_SIMULATOR_DDI_HANDLER_LRP_HXX

#include <initializer_list>
#include <memory>
#include <optional>

#include <windows.h>

#include <wdf.h>

#include "UwbSimulatorDdiCallbacksLrp.hxx"
#include "UwbSimulatorDdiHandler.hxx"
#include "UwbSimulatorDispatchEntry.hxx"

namespace windows::devices::uwb::simulator
{
class UwbSimulatorDdiHandlerLrp :
    public UwbSimulatorDdiHandler
{
public:
    UwbSimulatorDdiHandlerLrp();

    /**
     * @brief Indicates whether the specified i/o control code is handled by
     * this handler.
     *
     * @param ioControlCode The i/o control code to check.
     * @return true
     * @return false
     */
    bool
    HandlesIoControlCode(ULONG ioControlCode) override;

    /**
     * @brief
     *
     * @param request
     * @param ioControlCode
     * @param inputBufferLength
     * @param outputBufferLength
     * @return NTSTATUS
     */
    NTSTATUS
    ValidateRequest(WDFREQUEST request, ULONG ioControlCode, std::size_t inputBufferLength, std::size_t outputBufferLength) override;

    /**
     * @brief
     *
     * @param request
     * @param ioControlCode
     * @param inputBuffer
     * @param outputBuffer
     * @return NTSTATUS
     */
    NTSTATUS
    HandleRequest(WDFREQUEST request, ULONG ioControlCode, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer) override;

private:
    NTSTATUS
    OnUwbDeviceReset(WDFREQUEST, std::span<uint8_t>, std::span<uint8_t>);

    NTSTATUS
    OnUwbGetDeviceInformation(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

    NTSTATUS
    OnUwbGetDeviceCapabilities(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

    NTSTATUS
    OnUwbGetDeviceConfigurationParameters(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

    NTSTATUS
    OnUwbSetDeviceConfigurationParameters(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

    NTSTATUS
    OnUwbGetApplicationConfigurationParameters(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

    NTSTATUS
    OnUwbSetApplicationConfigurationParameters(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

    NTSTATUS
    OnUwbGetSessionCount(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

    NTSTATUS
    OnUwbSessionInitialize(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

    NTSTATUS
    OnUwbSessionDeinitialize(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

    NTSTATUS
    OnUwbGetSessionState(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

    NTSTATUS
    OnUwbSessionUpdateControllerMulticastList(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

    NTSTATUS
    OnUwbSessionStartRanging(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

    NTSTATUS
    OnUwbSessionStopRanging(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

    NTSTATUS
    OnUwbSessionGetRangingCount(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

    NTSTATUS
    OnUwbNotification(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

    std::optional<windows::devices::uwb::simulator::UwbSimulatorDispatchEntry<UwbSimulatorDdiHandlerLrp>>
    TryGetDispatchEntry(ULONG ioControlCode);

private:
    static const std::initializer_list<windows::devices::uwb::simulator::UwbSimulatorDispatchEntry<UwbSimulatorDdiHandlerLrp>> Dispatch;

private:
    std::unique_ptr<windows::devices::uwb::simulator::UwbSimulatorDdiCallbacksLrp> m_callbacks;
};
} // namespace windows::devices::uwb::simulator

#endif // UWB_SIMULATOR_DDI_HANDLER_LRP_HXX
