
#ifndef WINDOWS_DEVICE_ENUMERATOR
#define WINDOWS_DEVICE_ENUMERATOR

#include <string>
#include <vector>

// NB: This must come before any other Windows include
#include <windows.h>

#include <guiddef.h>

namespace windows
{
namespace devices
{
class DeviceEnumerator
{
public:
    static std::vector<std::wstring>
    GetDeviceInterfaceClassInstanceNames(const GUID& deviceInterfaceClassGuid) noexcept;
};
} // namespace devices
} // namespace windows

#endif // WINDOWS_DEVICE_ENUMERATOR