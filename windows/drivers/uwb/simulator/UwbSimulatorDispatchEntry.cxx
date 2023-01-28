
#include "UwbSimulatorDispatchEntry.hxx"

using namespace windows::devices::uwb::simulator;

NTSTATUS
UwbSimulatorDispatchEntry::GetRequestValidityStatus(std::size_t inputBufferSize, std::size_t outputBufferSize) const noexcept
{
    if (inputBufferSize < InputSizeMinimum || outputBufferSize < OutputSizeMinimum) {
        return STATUS_BUFFER_TOO_SMALL;
    } else if (inputBufferSize > InputSizeMaximum || outputBufferSize > OutputSizeMaximum) {
        return STATUS_INVALID_BUFFER_SIZE;
    } else {
        return STATUS_SUCCESS;
    }
}
