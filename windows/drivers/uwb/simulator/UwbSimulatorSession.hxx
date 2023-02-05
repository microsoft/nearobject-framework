
#ifndef UWB_SIMULATOR_SESSION_HXX
#define UWB_SIMULATOR_SESSION_HXX

#include <memory>
#include <vector>

#include <uwb/protocols/fira/FiraDevice.hxx>
#include <windows/devices/uwb/UwbAppConfiguration.hxx>

namespace windows::devices::uwb::simulator
{
struct UwbSimulatorSession
{
    UwbSessionState State{ UwbSessionState::Deinitialized };
    std::vector<std::shared_ptr<IUwbAppConfigurationParameter>> ApplicationConfigurationParameters;
};
} // namespace windows::devices::uwb::simulator

#endif // UWB_SIMULATOR_SESSION_HXX