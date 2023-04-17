
#ifndef UWBCX_DDI_LRP_HXX
#define UWBCX_DDI_LRP_HXX

// NB: This must come before any other Windows include
#include <stdint.h>
#include <windows.h>
#include <winioctl.h>

#include <windows/devices/uwb/UwbCxLrpDevice.h>

namespace windows::drivers::uwbcx
{
const std::string UwbNotificationNamespace = "\\Notifications";

} // namespace windows::drivers::uwbcx

#endif // UWBCX_DDI_LRP_HXX
