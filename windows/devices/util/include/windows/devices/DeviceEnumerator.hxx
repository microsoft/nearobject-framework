
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
/**
 * @brief Enumerates the names of device interface instances belonging to a
 * specified device interface class.
 */
struct DeviceEnumerator
{
    /**
     * @brief Get a list of device paths for instances of the specified device
     * interface class.
     * 
     * @param deviceInterfaceClassGuid The GUID of the device class to enumerate.
     * @return std::vector<std::string> A list of device paths.
     */
    static std::vector<std::string>
    GetDeviceInterfaceClassInstanceNames(const GUID& deviceInterfaceClassGuid) noexcept;
};
} // namespace devices
} // namespace windows

#endif // WINDOWS_DEVICE_ENUMERATOR