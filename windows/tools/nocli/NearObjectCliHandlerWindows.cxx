
#include <iostream>
#include <string>
#include <string_view>

#include <notstd/guid.hxx>

#include <windows/devices/DeviceEnumerator.hxx>
#include <windows/devices/DevicePresenceMonitor.hxx>
#include <windows/uwb/UwbDevice.hxx>

#include "NearObjectCliHandlerWindows.hxx"
#include "NearObjectCliDataWindows.hxx"

using namespace nearobject::cli;

namespace detail
{
using namespace windows::devices;

/**
 * @brief Get the default UWB device on the system.
 *
 * This enumerates all the UWB devices on the system and returns the name of the
 * first one.
 * 
 * @return std::string 
 */
std::string
GetUwbDeviceNameDefault() noexcept
{
    const auto uwbDeviceNames = DeviceEnumerator::GetDeviceInterfaceClassInstanceNames(windows::devices::uwb::InterfaceClassUwb);
    return !uwbDeviceNames.empty() ? uwbDeviceNames.front() : "";
}

/**
 * @brief Probes for uwb devices and prompts the user to select one.
 * 
 * @param deviceClassGuid 
 * @return std::string 
 */
std::string
GetUwbDeviceNameFromProbe(std::string_view deviceClassGuid = windows::devices::uwb::InterfaceClassUwbString)
{
    const auto uwbDeviceNames = DeviceEnumerator::GetDeviceInterfaceClassInstanceNames(deviceClassGuid);
    if (uwbDeviceNames.empty()) {
        return "";
    }

    int32_t index = 0;
    for (const auto& uwbDeviceName : uwbDeviceNames) {
        std::cout << "[" << index++ << "] " << uwbDeviceName << std::endl;
    }

    for (index = -1;;) {
        std::cout << "select the uwb device to use from the list above [0-" << uwbDeviceNames.size() - 1 << "]: ";
        std::cin >> index;
        if (index >= 0 && index < uwbDeviceNames.size()) {
            break;
        }
        if (std::cin.fail()) {
            std::cin.clear();
        }
        std::cout << "invalid device index specified; please enter an index between 0 and " << uwbDeviceNames.size() - 1 << std::endl;
    }

    return uwbDeviceNames[static_cast<std::size_t>(index)];
}

/**
 * @brief Tries to resolve the UwbDevice to use for ranging.
 * 
 * @param cliData 
 * @return std::shared_ptr<windows::devices::UwbDevice> 
 */
std::shared_ptr<windows::devices::UwbDevice>
ResolveUwbDevice(const nearobject::cli::NearObjectCliDataWindows& cliData)
{
    std::string deviceName = cliData.DeviceNameProbe
        ? GetUwbDeviceNameFromProbe(cliData.DeviceClassGuid)
        : cliData.DeviceName.value_or(GetUwbDeviceNameDefault());

    // Ensure a device name was set.
    if (deviceName.empty())  {
        return nullptr;
    }

    return std::make_unique<windows::devices::UwbDevice>(deviceName);
}
} // namespace detail

std::shared_ptr<::uwb::UwbDevice>
NearObjectCliHandlerWindows::ResolveUwbDevice(const nearobject::cli::NearObjectCliData& cliData) noexcept
{
    const auto* cliDataWindows = dynamic_cast<const NearObjectCliDataWindows*>(&cliData);
    if (!cliDataWindows) {
        return nullptr;
    }

    return detail::ResolveUwbDevice(*cliDataWindows);
}
