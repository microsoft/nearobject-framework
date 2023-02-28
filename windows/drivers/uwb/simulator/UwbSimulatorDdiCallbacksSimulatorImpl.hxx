
#ifndef UWB_SIMULATOR_DDI_CALLBACKS_SIMULATOR_IMPL
#define UWB_SIMULATOR_DDI_CALLBACKS_SIMULATOR_IMPL

#include <windows.h>

#include "UwbSimulatorDdi.h"
#include "UwbSimulatorDdiCallbacksSimulator.hxx"

namespace windows::devices::uwb::simulator
{
/**
 * @brief
 */
struct UwbSimulatorDdiCallbacksSimulatorImpl :
    public UwbSimulatorDdiCallbacksSimulator
{
    virtual ~UwbSimulatorDdiCallbacksSimulatorImpl() = default;

    virtual void
    GetSimulatorCapabilities(UwbSimulatorCapabilities &uwbSimulatorCapabilities) override;

    virtual void
    TriggerSessionEvent(const UwbSimulatorTriggerSessionEventArgs &triggerSessionEventArgs) override;
};
} // namespace windows::devices::uwb::simulator

#endif // UWB_SIMULATOR_DDI_CALLBACKS_SIMULATOR_IMPL
