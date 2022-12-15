
#ifndef WINDOWS_DEVICE_ENUMERATOR
#define WINDOWS_DEVICE_ENUMERATOR

#include <string_view>
#include <vector>

// NB: This must come before any other Windows include
#include <windows.h>

#include <guiddef.h>

namespace windows::devices
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

    /**
     * @brief Get a list of device paths for instances of the specified device
     * interface class.
     *
     * @param deviceInterfaceClassString
     * @return std::vector<std::string>
     */
    static std::vector<std::string>
    GetDeviceInterfaceClassInstanceNames(std::string_view deviceInterfaceClassString) noexcept;
};
} // namespace windows::devices

#endif // WINDOWS_DEVICE_ENUMERATOR
