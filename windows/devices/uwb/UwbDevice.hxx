
#ifndef WINDOWS_DEVICE_UWB_HXX
#define WINDOWS_DEVICE_UWB_HXX

#include <uwb/UwbDevice.hxx>

namespace windows
{
namespace devices
{

/**
 * @brief Helper class to interact with Windows UWB devices using the Windows
 * UWB DDI. The DDI is to be determined.
 */
class UwbDevice :
    public uwb::UwbDevice
{
};

} // namespace devices
} // namespace windows

#endif // WINDOWS_DEVICE_UWB_HXX
