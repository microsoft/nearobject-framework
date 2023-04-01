
#ifndef I_UWB_DEVICE_SIMULATOR_DDI_HXX
#define I_UWB_DEVICE_SIMULATOR_DDI_HXX

#include <future>

#include <UwbSimulatorDdiGlue.h>

namespace windows::devices::uwb::simulator
{
struct IUwbDeviceSimulatorDdi
{
    // IOCTL_UWB_DEVICE_SIM_GET_CAPABILITIES
    virtual std::future<UwbSimulatorCapabilities>
    GetCapabilites() = 0;

    // IOCTL_UWB_DEVICE_SIM_TRIGGER_SESSION_EVENT
    virtual std::future<UwbSimulatorTriggerSessionEventResult>
    TriggerSessionEvent(const UwbSimulatorTriggerSessionEventArgs& triggerSessionEventArgs) = 0;
};
} // namespace windows::devices::uwb::simulator

#endif // I_UWB_DEVICE_SIMULATOR_DDI_HXX
