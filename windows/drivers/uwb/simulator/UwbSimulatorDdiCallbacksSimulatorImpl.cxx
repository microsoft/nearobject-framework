
#include <windows.h>

#include "UwbSimulatorDdiCallbacksSimulatorImpl.hxx"
#include "UwbSimulatorTracelogging.hxx"

using namespace windows::devices::uwb;
using namespace windows::devices::uwb::simulator;

UwbSimulatorCapabilities
UwbSimulatorDdiCallbacksSimulatorImpl::GetSimulatorCapabilities()
{
	return {};
}

void
UwbSimulatorDdiCallbacksSimulatorImpl::TriggerSessionEvent(const UwbSimulatorTriggerSessionEventArgs &triggerSessionEventArgs)
{
	switch (triggerSessionEventArgs.Action) {
    case UwbSimulatorSessionEventAction::RandomRangingMeasurementGenerationStart: {
        break;
	}
    case UwbSimulatorSessionEventAction::RandomRangingMeasurementGenerationStop: {
        break;
    }
    case UwbSimulatorSessionEventAction::None: {
        break;
    }
    default: {
        break;
    }
	}
}
