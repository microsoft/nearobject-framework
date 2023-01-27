
#include "UwbCxLrpDeviceGlue.h"

#include "UwbSimulatorDdiCallbacksLrpNoop.hxx"
#include "UwbSimulatorDdiHandlerLrp.hxx"

using namespace windows::devices::uwb::simulator;

UwbSimulatorDdiHandlerLrp::UwbSimulatorDdiHandlerLrp() :
    m_callbacks(std::make_unique<UwbSimulatorDdiCallbacksLrpNoop>())
{
}

bool
UwbSimulatorDdiHandlerLrp::HandlesIoControlCode(ULONG ioControlCode)
{
    switch (ioControlCode) {
    case IOCTL_UWB_DEVICE_RESET:
    case IOCTL_UWB_GET_APP_CONFIG_PARAMS:
    case IOCTL_UWB_GET_DEVICE_CAPABILITIES:
    case IOCTL_UWB_GET_DEVICE_CONFIG_PARAMS:
    case IOCTL_UWB_GET_DEVICE_INFO:
    case IOCTL_UWB_GET_RANGING_COUNT:
    case IOCTL_UWB_GET_SESSION_COUNT:
    case IOCTL_UWB_GET_SESSION_STATE:
    case IOCTL_UWB_NOTIFICATION:
    case IOCTL_UWB_SESSION_DEINIT:
    case IOCTL_UWB_SESSION_INIT:
    case IOCTL_UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST:
    case IOCTL_UWB_SET_APP_CONFIG_PARAMS:
    case IOCTL_UWB_SET_DEVICE_CONFIG_PARAMS:
    case IOCTL_UWB_START_RANGING_SESSION:
    case IOCTL_UWB_STOP_RANGING_SESSION:
        return true;
    }

    return false;
}

NTSTATUS
UwbSimulatorDdiHandlerLrp::ValidateRequest(WDFREQUEST /*request*/, ULONG /*ioControlCode*/, std::size_t /*inputBufferLength*/, std::size_t /*outputBufferLength*/)
{
    return STATUS_NOT_SUPPORTED;
}

NTSTATUS
UwbSimulatorDdiHandlerLrp::HandleRequest(WDFREQUEST /* request */, ULONG /* ioControlCode */, std::span<uint8_t> /* inputBuffer */, std::span<uint8_t> /* outputBuffer */)
{
    return STATUS_NOT_IMPLEMENTED;
}
