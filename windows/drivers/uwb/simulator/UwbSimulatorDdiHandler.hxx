
#ifndef UWB_SIMULATOR_DDI_HANDLER_HXX
#define UWB_SIMULATOR_DDI_HANDLER_HXX

#include <cstddef>
#include <span>

#include <windows.h>

#include <wdf.h>

namespace windows::devices::uwb::simulator
{
/**
 * @brief Base class which responsible for handling DDI i/o control requests.
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
    HandlesIoControlCode(ULONG ioControlCode) = 0;

    /**
     * @brief Validates that a given request is valid.
     *
     * This is responsible for validating that the input and output buffers are
     * of sufficient length.
     *
     * @param request
     * @param ioControlCode
     * @param inputBufferLength
     * @param outputBufferLength
     * @return NTSTATUS
     */
    virtual NTSTATUS
    ValidateRequest(WDFREQUEST request, ULONG ioControlCode, std::size_t inputBufferLength, std::size_t outputBufferLength) = 0;

    /**
     * @brief Handles the request.
     *
     * @param request
     * @param ioControlCode
     * @param inputBuffer
     * @param outputBuffer
     * @return NTSTATUS
     */
    virtual NTSTATUS
    HandleRequest(WDFREQUEST request, ULONG ioControlCode, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer) = 0;
};
} // namespace windows::devices::uwb::simulator

#endif // UWB_SIMULATOR_DDI_HANDLER_HXX
