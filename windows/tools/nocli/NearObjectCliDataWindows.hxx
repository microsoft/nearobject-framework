
#ifndef NEAR_OBJECT_CLI_DATA_WINDOWS_HXX
#define NEAR_OBJECT_CLI_DATA_WINDOWS_HXX

#include <optional>
#include <string>

#include <nearobject/cli/NearObjectCliData.hxx>
#include <windows/uwb/UwbDeviceDriver.hxx>

namespace nearobject::cli
{
/**
 * @brief Base class for data parsed by NearObjectCli.
 */
struct NearObjectCliDataWindows :
    public NearObjectCliData
{
    /**
     * @brief Controls whether a probe of the active devices will be done,
     * allowing the caller to select (by console input), the device to use.
     * 
     * This will scan for devices with the GUID specified by DeviceClassGuid.
     */
    bool DeviceNameProbe{ false };

    /**
     * @brief The device name (path) of the UWB device to use. If not specified,
     * a default device will be automatically selected.
     */
    std::optional<std::string> DeviceName;

    /**
     * @brief The class GUID of the device to use. This defaults to the UWB
     * Class Extension (UwbCx) Driver GUID, but can be overriden using this
     * option. 
     */
    std::string DeviceClassGuid{ windows::devices::uwb::InterfaceClassUwbString };
};
} // namespace nearobject::cli::windows

#endif // NEAR_OBJECT_CLI_WINDOWS_HXX
