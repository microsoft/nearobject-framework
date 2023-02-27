
#ifndef UWB_SIMULATOR_DDI_HANDLER_SIMULATOR_HXX
#define UWB_SIMULATOR_DDI_HANDLER_SIMULATOR_HXX

#include <initializer_list>
#include <memory>
#include <optional>

#include <windows.h>

#include <wdf.h>

#include "UwbSimulatorDdiCallbacksSimulator.hxx"
#include "UwbSimulatorDdiHandler.hxx"
#include "UwbSimulatorDispatchEntry.hxx"

namespace windows::devices::uwb::simulator
{
class UwbSimulatorDdiHandlerSimulator :
    public UwbSimulatorDdiHandler
{
public:
    UwbSimulatorDdiHandlerSimulator();

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
    OnUwbSimulatorCapabilities(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

    NTSTATUS
    OnUwbSimulatorTriggerSessionEvent(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

    std::optional<windows::devices::uwb::simulator::UwbSimulatorDispatchEntry<UwbSimulatorDdiHandlerSimulator>>
    TryGetDispatchEntry(ULONG ioControlCode);

private:
    static const std::initializer_list<windows::devices::uwb::simulator::UwbSimulatorDispatchEntry<UwbSimulatorDdiHandlerSimulator>> Dispatch;

private:
    std::unique_ptr<windows::devices::uwb::simulator::UwbSimulatorDdiCallbacksSimulator> m_callbacks;
};
} // namespace windows::devices::uwb::simulator

#endif // UWB_SIMULATOR_DDI_HANDLER_SIMULATOR_HXX
