
#ifndef UWB_DEVICE_DRIVER_HXX
#define UWB_DEVICE_DRIVER_HXX

#include <initguid.h>
#include <guiddef.h>

namespace windows
{
namespace devices
{
namespace uwb
{
#ifndef GUID_DEVINTERFACE_UWB
    // TODO: remove once the actual driver interface is defined and exposes this officially
    // 1f49c35a-f284-4f81-87d7-4725a6eec3e8
    DEFINE_GUID(GUID_DEVINTERFACE_UWB, 0x1f49c35a, 0xf284, 0x4f81, 0x87, 0xd7, 0x47, 0x25, 0xa6, 0xee, 0xc3, 0xe8);
#endif

const GUID InterfaceClassUwb = GUID_DEVINTERFACE_UWB;
}
}
} // namespace windows

#endif // UWB_DEVICE_DRIVER_HXX
