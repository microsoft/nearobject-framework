
#ifndef UWB_SIMULATOR_SESSION_HXX
#define UWB_SIMULATOR_SESSION_HXX

#include <memory>
#include <vector>

#include <windows/devices/uwb/UwbAppConfiguration.hxx>

namespace windows::devices::uwb::simulator
{
struct UwbSimulatorSession
{
    std::vector<std::shared_ptr<IUwbAppConfigurationParameter>> AapplicationConfigurationParameters;
};
} // namespace windows::devices::uwb::simulator

#endif // UWB_SIMULATOR_SESSION_HXX