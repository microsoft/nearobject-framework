
#ifndef UWB_SIMULATOR_DDI_HANDLER_HXX 
#define UWB_SIMULATOR_DDI_HANDLER_HXX

#include <cstddef>

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

    /**
     * @brief 
     * 
     * @param request 
     * @param outputBufferLength 
     * @param inputBufferLength 
     * @param ioControlCode 
     * @return NTSTATUS 
     */
    virtual NTSTATUS
    HandleRequest(WDFREQUEST request, std::size_t outputBufferLength, std::size_t inputBufferLength, ULONG ioControlCode) = 0;
};
} // namespace windows::devices::uwb::simulator

#endif // UWB_SIMULATOR_DDI_HANDLER_HXX
