
#ifndef UWB_SIMULATOR_DDI_HANDLER_LRP_HXX
#define UWB_SIMULATOR_DDI_HANDLER_LRP_HXX

#include <windows.h>

#include <wdf.h>

#include "UwbSimulatorDdiHandler.hxx"
#include "UwbSimulatorDdiCallbacksLrp.hxx"

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
    HandlesIoControlCode(ULONG ioControlCode) const noexcept override;

    /**
     * @brief 
     * 
     * @param request 
     * @param outputBufferLength 
     * @param inputBufferLength 
     * @param ioControlCode 
     * @return NTSTATUS 
     */
    NTSTATUS
    HandleRequest(WDFREQUEST request, std::span<uint8_t> outputBuffer, std::span<uint8_t> inputBuffer, ULONG ioControlCode) override;

private:
    std::unique_ptr<windows::devices::uwb::simulator::UwbSimulatorDdiCallbacksLrp> m_callbacks;
};
} // namespace windows::devices::uwb::simulator

#endif // UWB_SIMULATOR_DDI_HANDLER_LRP_HXX
