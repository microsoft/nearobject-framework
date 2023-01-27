
#include "UwbSimulatorDdiHandlerLrp.hxx"

using namespace windows::devices::uwb::simulator;

bool
UwbSimulatorDdiHandlerLrp::HandlesIoControlCode(ULONG /* ioControlCode */) const noexcept
{
    return false; 
}

NTSTATUS
UwbSimulatorDdiHandlerLrp::HandleRequest(WDFREQUEST /* request */, std::size_t /* outputBufferLength */, std::size_t /* inputBufferLength */, ULONG /* ioControlCode */)
{
    return STATUS_NOT_IMPLEMENTED; 
}
