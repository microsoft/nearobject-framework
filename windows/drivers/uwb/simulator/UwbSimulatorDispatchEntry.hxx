
#ifndef UWB_SIMULATOR_DISPATCH_ENTRY_HXX
#define UWB_SIMULATOR_DISPATCH_ENTRY_HXX

#include <cstddef>
#include <functional>
#include <limits>
#include <span>

#include <windows.h>

#include <wdf.h>

namespace windows::devices::uwb::simulator
{
/**
 * @brief Represents an entry used to dispatch WDF requests to a handler
 * implementation.
 * 
 * @tparam ClassT The type of the containing class which defines the
 * implementation handler functions. This limits the member functions which can
 * be used to dispatch execution.
 */
template <typename ClassT>
struct UwbSimulatorDispatchEntry
{
    using HandlerFuncT = NTSTATUS (ClassT::*)(WDFREQUEST, std::span<uint8_t>, std::span<uint8_t>);

    ULONG IoControlCode;
    std::size_t InputSizeMinimum{ 0 };
    std::size_t InputSizeMaximum{ std::numeric_limits<std::size_t>::max() };
    std::size_t OutputSizeMinimum{ 0 };
    std::size_t OutputSizeMaximum{ std::numeric_limits<std::size_t>::max() };
    HandlerFuncT Handler;

    /**
     * @brief Determines whether the specified request parameters are valid for
     * this dispatch entry.
     * 
     * @param inputBufferSize The size of the request input buffer, in bytes.
     * @param outputBufferSize The size of the request output buffer, in bytes.
     * @return NTSTATUS
     */
    NTSTATUS
    GetRequestValidityStatus(std::size_t inputBufferSize, std::size_t outputBufferSize) const noexcept
    {
        if (inputBufferSize < InputSizeMinimum || outputBufferSize < OutputSizeMinimum) {
            return STATUS_BUFFER_TOO_SMALL;
        } else if (inputBufferSize > InputSizeMaximum || outputBufferSize > OutputSizeMaximum) {
            return STATUS_INVALID_BUFFER_SIZE;
        } else {
            return STATUS_SUCCESS;
        }
    }
};
} // namespace windows::devices::uwb::simulator

#endif // UWB_SIMULATOR_DISPATCH_ENTRY_HXX
