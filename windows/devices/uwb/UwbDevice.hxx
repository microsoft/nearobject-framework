
#ifndef __WINDOWS_DEVICE_UWB_HXX__
#define __WINDOWS_DEVICE_UWB_HXX__

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

#endif // __WINDOWS_DEVICE_UWB_HXX__
