
#include <cstdint>
#include <iostream>
#include <memory>
#include <optional>
#include <string>

#include <CLI/CLI.hpp>
#include <magic_enum.hpp>
#include <notstd/guid.hxx>

#include "NearObjectCliDataWindows.hxx"
#include "NearObjectCliUwbSessionEventCallbacks.hxx"
#include <nearobject/cli/NearObjectCli.hxx>
#include <windows/devices/DeviceEnumerator.hxx>
#include <windows/devices/DevicePresenceMonitor.hxx>
#include <windows/uwb/UwbDevice.hxx>

#include <optional>

using namespace windows::devices;

namespace detail
{
/**
 * @brief Get the default UWB device on the system.
 *
 * This enumerates all the UWB devices on the system and returns the name of the
 * first one.
 *
 * @return std::optional<std::string>
 */
std::optional<std::string>
GetDefaultUwbDeviceName() noexcept
{
    const auto uwbDeviceNames = DeviceEnumerator::GetDeviceInterfaceClassInstanceNames(windows::devices::uwb::InterfaceClassUwb);
    return !uwbDeviceNames.empty()
        ? std::optional<std::string>(uwbDeviceNames.front())
        : std::nullopt;
}

/**
 * @brief The action to take when the --probe flag is passed via cli
 * 
 * @param cliData 
 */
void
DeviceNameProbeAction(nearobject::cli::NearObjectCliDataWindows& cliData)
{
    if (cliData.DeviceName.has_value()) {
        std::cout << "warning: device name '" << cliData.DeviceName.value() << "' will be ignored due to device name probe request" << std::endl;
    }

    const auto uwbDeviceNames = DeviceEnumerator::GetDeviceInterfaceClassInstanceNames(cliData.DeviceClassGuid);
    if (!uwbDeviceNames.empty()) {
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

        cliData.DeviceName = uwbDeviceNames[static_cast<std::size_t>(index)];
    }
}

/**
 * @brief tries to resolve the UwbDevice to use for ranging
 * 
 * @param cliData 
 * @return std::unique_ptr<windows::devices::UwbDevice> 
 */
std::unique_ptr<windows::devices::UwbDevice>
ResolveUwbDevice(nearobject::cli::NearObjectCliDataWindows& cliData)
{
    if (cliData.DeviceNameProbe) {
        DeviceNameProbeAction(cliData);
    }

    // Ensure a device name was set.
    if (!cliData.DeviceName.has_value()) {
        cliData.DeviceName = GetDefaultUwbDeviceName();
        if (!cliData.DeviceName) {
            std::cerr << "error: no uwb device could be found" << std::endl;
            return nullptr;
        }
    }

    std::cout << "Using UWB device " << cliData.DeviceName.value() << std::endl;
    auto uwbDevice = std::make_unique<windows::devices::UwbDevice>(cliData.DeviceName.value());
    if (!uwbDevice) {
        std::cerr << "error: failed to create instance of uwb device " << cliData.DeviceName.value() << std::endl;
        return nullptr;
    }
    return uwbDevice;
}

} // namespace detail

int
main(int argc, char* argv[])
{
    auto cliData = std::make_shared<nearobject::cli::NearObjectCliDataWindows>();
    nearobject::cli::NearObjectCli cli{ cliData };
    CLI::App& topApp = cli.GetParser();

    auto uwbApp = cli.GetUwbApp();
    auto rangeApp = cli.GetRangeApp();
    auto startRangingApp = cli.GetRangeStartApp();

    // Configure the cli parsing app with Windows-specific options.
    uwbApp->add_option("--deviceName", cliData->DeviceName, "uwb device name (path)");
    uwbApp->add_option("--deviceClass", cliData->DeviceClassGuid, "uwb device class guid (override)");
    uwbApp->add_flag("--probe", cliData->DeviceNameProbe, "probe for the uwb device name to use");

    startRangingApp->final_callback([&cliData] {
        auto uwbDevice = detail::ResolveUwbDevice(*cliData);
        if (!uwbDevice) {
            return;
        }

        auto callbacks = std::make_shared<nearobject::cli::NearObjectCliUwbSessionEventCallbacks>();
        auto session = uwbDevice->CreateSession(callbacks);
        session->Configure(cliData->SessionData);
        session->StartRanging();
    });

    // Parse the arguments.
    int result = cli.Parse(argc, argv);
    if (result != 0) {
        return result;
    }

    return 0;
}
