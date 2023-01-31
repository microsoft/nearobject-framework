
#include <iostream>
#include <limits>
#include <optional>
#include <vector>

#include <CLI/CLI.hpp>
#include <magic_enum.hpp>
#include <notstd/guid.hxx>

#include <windows/devices/DeviceEnumerator.hxx>
#include <windows/devices/DevicePresenceMonitor.hxx>

#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Log.h>

int
main(int argc, char* argv[])
{
    plog::init(plog::verbose, "devicemon_log.txt");

    CLI::App app{};
    app.name("devicemon");
    app.description("monitor device class instances for presence changes");

    bool initialProbe = false;
    std::string deviceGuidString{};
    app.add_option("-g,--guid", deviceGuidString, "The GUID of the device interface class to monitor");
    app.add_flag("-p,--probe", initialProbe, "perform an initial probe for currently attached device instances");

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError& e) {
        std::cerr << "failed to parse command line arguments" << std::endl;
        return app.exit(e);
    }

    // If brace-formatted version supplied, remove the braces as the conversion
    // function requires them not to be present.
    if (deviceGuidString.front() == '{') {
        deviceGuidString.erase(std::begin(deviceGuidString));
    }
    if (deviceGuidString.back() == '}') {
        deviceGuidString.pop_back();
    }

    const auto deviceGuid = notstd::GuidFromString(deviceGuidString);
    if (!deviceGuid) {
        std::cerr << "invalid device class guid specified (" << deviceGuidString << ")" << std::endl;
        return -1;
    }

    if (initialProbe) {
        const auto deviceNames = windows::devices::DeviceEnumerator::GetDeviceInterfaceClassInstanceNames(deviceGuid.value());
        if (!deviceNames.empty()) {
            std::cout << "Current device instances present: " << std::endl;
            for (const auto& deviceName : deviceNames) {
                std::cout << deviceName << std::endl;
            }
            std::cout << std::endl;
        }
    }

    windows::devices::DevicePresenceMonitor monitor{deviceGuid.value(), [](auto&& presenceEvent, auto&& deviceName) {
        const auto presenceEventName = magic_enum::enum_name(presenceEvent);
        std::cout << deviceName << " " << presenceEventName << std::endl;
    }};

    monitor.Start();

    std::cout << "Monitoring events for the device with class GUID " << deviceGuidString << ". Press any key to exit." << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return 0;
}
