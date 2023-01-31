
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>

#include <CLI/CLI.hpp>
#include <notstd/guid.hxx>

#include <UwbSimulatorDdiGlue.h>
#include <windows/devices/DeviceEnumerator.hxx>
#include <windows/devices/uwb/simulator/UwbDeviceSimulator.hxx>

#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Log.h>

using namespace windows::devices;

int
main(int argc, char* argv[])
{
    plog::init(plog::verbose, "uwbsim_log.txt");

    CLI::App app{};
    app.name("uwbsim");
    app.description("control and interact uwb simulator devices");

    std::string deviceName{};
    app.add_option("--deviceName, -d", deviceName, "The uwb simulator device name (path)");

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError& e) {
        std::cerr << "failed to parse command line arguments" << std::endl;
        return app.exit(e);
    }

    if (deviceName.empty()) {
        const auto deviceNames = DeviceEnumerator::GetDeviceInterfaceClassInstanceNames(GUID_DEVINTERFACE_UWB_SIMULATOR);
        if (!deviceNames.empty()) {
            deviceName = deviceNames.front();
        }
    }

    if (deviceName.empty()) {
        std::cerr << "no uwb simulator device found or no device specified" << std::endl;
        return -1;
    }

    std::cout << "creating uwb simulator device " << deviceName << std::endl;

    auto uwbDeviceSimulator = std::make_unique<uwb::simulator::UwbDeviceSimulator>(deviceName);
    if (!uwbDeviceSimulator) {
        std::cerr << "failed to create uwb simulator device instance" << std::endl;
        return -2;
    }

    if (!uwbDeviceSimulator->Initialize()) {
        std::cerr << "failed to initialize uwb simulator device" << std::endl;
        return -2;
    }

    std::cout << "initialization succeeded" << std::endl;

    return 0;
}
