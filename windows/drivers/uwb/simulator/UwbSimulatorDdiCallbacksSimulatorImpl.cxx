
#include <windows.h>

#include "UwbSimulatorDdiCallbacksSimulatorImpl.hxx"
#include "UwbSimulatorTracelogging.hxx"

using namespace windows::devices::uwb;
using namespace windows::devices::uwb::simulator;

void
UwbSimulatorDdiCallbacksSimulatorImpl::GetSimulatorCapabilities(UwbSimulatorCapabilities & /*uwbSimulatorCapabilities*/)
{
}

void
UwbSimulatorDdiCallbacksSimulatorImpl::TriggerSessionEvent(const UwbSimulatorTriggerSessionEventArgs & /*triggerSessionEventArgs*/)
{
}
