
#ifndef UWB_SIMULATOR_DDI_HANDLER_HXX 
#define UWB_SIMULATOR_DDI_HANDLER_HXX

#include <cstddef>
#include <span>

#include <windows.h>

#include <wdf.h>

namespace windows::devices::uwb::simulator
{
/**
 * @brief Base class which is responsible for handling DDI i/o control
 * requests.
 */
struct UwbSimulatorDdiHandler
{
    virtual ~UwbSimulatorDdiHandler() = default;

    /**
     * @brief Indicates whether the specified i/o control code is handled by
     * this handler. 
     * 
     * @param ioControlCode The i/o control code to check.
     * @return true 
     * @return false 
     */
    virtual bool
    HandlesIoControlCode(ULONG ioControlCode) const noexcept = 0;

    // virtual NTSTATUS
    // ValidateRequest(WDFREQUEST request, std::size_t outputBufferLength, )

    /**
     * @brief 
     * 
     * @param request 
     * @param outputBuffer 
     * @param inputBuffer 
     * @param ioControlCode 
     * @return NTSTATUS 
     */
    virtual NTSTATUS
    HandleRequest(WDFREQUEST request, std::span<uint8_t> outputBuffer, std::span<uint8_t> inputBuffer, ULONG ioControlCode) = 0;
};
} // namespace windows::devices::uwb::simulator

#endif // UWB_SIMULATOR_DDI_HANDLER_HXX
