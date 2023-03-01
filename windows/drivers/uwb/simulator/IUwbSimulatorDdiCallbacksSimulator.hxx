
#ifndef UWB_SIMULATOR_DDI_CALLBACKS_SIMULATOR
#define UWB_SIMULATOR_DDI_CALLBACKS_SIMULATOR

#include <windows.h>

#include "UwbSimulatorDdi.h"

namespace windows::devices::uwb::simulator
{
/**
 * @brief
 */
struct IUwbSimulatorDdiCallbacksSimulator
{
    virtual ~IUwbSimulatorDdiCallbacksSimulator() = default;

    /**
     * @brief
     *
     * @return UwbStatus
     */
    virtual UwbSimulatorCapabilities
    GetSimulatorCapabilities() = 0;

    /**
     * @brief
     *
     * @param deviceInfo
     * @return UwbStatus
     */
    virtual void
    TriggerSessionEvent(const UwbSimulatorTriggerSessionEventArgs &triggerSessionEventArgs) = 0;
};
} // namespace windows::devices::uwb::simulator

#endif // UWB_SIMULATOR_DDI_CALLBACKS_SIMULATOR
