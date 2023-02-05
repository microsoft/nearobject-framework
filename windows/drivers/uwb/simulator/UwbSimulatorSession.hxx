
#ifndef UWB_SIMULATOR_SESSION_HXX
#define UWB_SIMULATOR_SESSION_HXX

#include <memory>
#include <unordered_set>
#include <vector>

#include <uwb/protocols/fira/FiraDevice.hxx>
#include <uwb/UwbMacAddress.hxx>
#include <windows/devices/uwb/UwbAppConfiguration.hxx>

namespace windows::devices::uwb::simulator
{
struct UwbSimulatorSession
{
    uint32_t Id;
    UwbSessionType Type{ UwbSessionType::RangingSession };
    UwbSessionState State{ UwbSessionState::Deinitialized };
    uint32_t Sequence{ 0 };
    std::unordered_set<UwbMacAddress> Controlees;
    std::vector<std::shared_ptr<IUwbAppConfigurationParameter>> ApplicationConfigurationParameters;
};
} // namespace windows::devices::uwb::simulator

#endif // UWB_SIMULATOR_SESSION_HXX