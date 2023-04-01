
#ifndef I_UWB_SIMULATOR_HXX
#define I_UWB_SIMULATOR_HXX

#include <cstdint>

namespace windows::devices::uwb::simulator
{
struct IUwbSimulator
{
    static constexpr uint8_t VersionMajor = 1;
    static constexpr uint8_t VersionMinor = 0;
    static constexpr uint32_t Version = (VersionMajor << 16U) | VersionMinor;
};
} // namespace windows::devices::uwb::simulator

#endif // I_UWB_SIMULATOR_HXX
