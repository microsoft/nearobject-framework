
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
struct UwbSimulatorDispatchEntry
{
    ULONG IoControlCode;

    std::size_t InputSizeMinimum{ 0 };
    std::size_t InputSizeMaximum{ std::numeric_limits<std::size_t>::max() };
    std::size_t OutputSizeMinimum{ 0 };
    std::size_t OutputSizeMaximum{ std::numeric_limits<std::size_t>::max() };
    
    std::function<NTSTATUS(WDFREQUEST, ULONG, std::span<uint8_t>, std::span<uint8_t>)> Handler;

    NTSTATUS
    GetRequestValidityStatus(std::size_t inputBufferSize, std::size_t outputBufferSize) const noexcept;
};
} // namespace windows::devices::uwb::simulator

#endif // UWB_SIMULATOR_DISPATCH_ENTRY_HXX
