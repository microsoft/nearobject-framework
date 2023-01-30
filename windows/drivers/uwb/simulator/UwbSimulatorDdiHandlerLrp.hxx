
#ifndef UWB_SIMULATOR_DDI_HANDLER_LRP_HXX
#define UWB_SIMULATOR_DDI_HANDLER_LRP_HXX

#include <windows.h>

#include <wdf.h>

#include "UwbSimulatorDdiCallbacksLrp.hxx"
#include "UwbSimulatorDdiHandler.hxx"

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
    std::unique_ptr<windows::devices::uwb::simulator::UwbSimulatorDdiCallbacksLrp> m_callbacks;
};
} // namespace windows::devices::uwb::simulator

#endif // UWB_SIMULATOR_DDI_HANDLER_LRP_HXX
