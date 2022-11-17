
#ifndef UWB_DEVICE_DRIVER_HXX
#define UWB_DEVICE_DRIVER_HXX

#include <initguid.h>

namespace windows
{
namespace devices
{
namespace uwb
{
// TODO: remove once the driver DDI is exposed through the Windows SDK/WDK.
#ifndef GUID_UWB_DEVICE_INTERFACE
// {A7424370-45E5-49C3-AE35-42A3C41B34AD}
DEFINE_GUID(GUID_UWB_DEVICE_INTERFACE, 0xa7424370, 0x45e5, 0x49c3, 0xae, 0x35, 0x42, 0xa3, 0xc4, 0x1b, 0x34, 0xad);
#endif

const GUID InterfaceClassUwb = GUID_UWB_DEVICE_INTERFACE;
} // namespace uwb
} // namespace devices
} // namespace windows

#endif // UWB_DEVICE_DRIVER_HXX
