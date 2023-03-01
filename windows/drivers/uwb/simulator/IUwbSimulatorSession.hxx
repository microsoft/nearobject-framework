
#ifndef I_UWB_SIMULATOR_SESSION_HXX
#define I_UWB_SIMULATOR_SESSION_HXX

#include <memory>
#include <unordered_set>
#include <vector>

#include <uwb/UwbMacAddress.hxx>
#include <uwb/protocols/fira/FiraDevice.hxx>
#include <windows/devices/uwb/UwbAppConfiguration.hxx>

namespace windows::devices::uwb::simulator
{
struct IUwbSimulatorSession
{
    uint32_t Id;
    UwbSessionType Type{ UwbSessionType::RangingSession };
    UwbSessionState State{ UwbSessionState::Deinitialized };
    uint32_t Sequence{ 0 };
    std::unordered_set<UwbMacAddress> Controlees;
    std::vector<std::shared_ptr<IUwbAppConfigurationParameter>> ApplicationConfigurationParameters;
};
} // namespace windows::devices::uwb::simulator

#endif // I_UWB_SIMULATOR_SESSION_HXX
