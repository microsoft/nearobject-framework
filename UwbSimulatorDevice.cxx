
#include "UwbSimulatorDevice.hxx"
#include "UwbSimulatorTracelogging.hxx"

/* static */
void
UwbSimulatorDevice::OnFileCreate(WDFDEVICE device, WDFREQUEST request, WDFFILEOBJECT file)
{
    TraceLoggingWrite(
        UwbSimulatorTraceloggingProvider, 
        "OnFileCreate", 
        TraceLoggingLevel(TRACE_LEVEL_INFORMATION),
        TraceLoggingPointer(device, "Device"),
        TraceLoggingPointer(request, "Request"),
        TraceLoggingPointer(file, "File")
    );
}

void
UwbSimulatorDevice::OnFileClose(WDFFILEOBJECT file)
{
    TraceLoggingWrite(
        UwbSimulatorTraceloggingProvider,
        "OnFileClose",
        TraceLoggingLevel(TRACE_LEVEL_INFORMATION),
        TraceLoggingPointer(file, "File"));
}
