
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

#include <CLI/CLI.hpp>
#include <notstd/guid.hxx>

#include <UwbSimulatorDdiGlue.h>
#include <uwb/protocols/fira/FiraDevice.hxx>
#include <uwb/protocols/fira/UwbException.hxx>
#include <windows/devices/DeviceEnumerator.hxx>
#include <windows/devices/uwb/simulator/UwbDeviceSimulator.hxx>

#include <plog/Appenders/DebugOutputAppender.h>
#include <plog/Appenders/RollingFileAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Init.h>
#include <plog/Log.h>

#include <logging/LogUtils.hxx>

using windows::devices::DeviceEnumerator;
using namespace ::uwb::protocol::fira;
using namespace windows::devices::uwb::simulator;

int
main(int argc, char* argv[])
{
    plog::RollingFileAppender<plog::TxtFormatter> rollingFileAppender(logging::GetLogName("uwbsim").c_str());
    plog::DebugOutputAppender<plog::TxtFormatter> debugAppender;
    plog::init(plog::verbose, &rollingFileAppender).addAppender(&debugAppender);

    CLI::App app{};
    app.name("uwbsim");
    app.description("control and interact with uwb simulator devices");

    bool getCapabilities{ false };
    std::string deviceName{};
    app.add_option("--deviceName, -d", deviceName, "The uwb simulator device name (path)");
    app.add_flag("--getCapabilities", getCapabilities, "enumerate the capabilities of the simulator");

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

    auto uwbDeviceSimulator = std::make_unique<UwbDeviceSimulator>(deviceName);
    if (!uwbDeviceSimulator) {
        std::cerr << "failed to create uwb simulator device instance" << std::endl;
        return -2;
    }

    if (!uwbDeviceSimulator->Initialize()) {
        std::cerr << "failed to initialize uwb simulator device" << std::endl;
        return -2;
    }

    if (getCapabilities) {
        try {
            auto capabilities = uwbDeviceSimulator->GetSimulatorCapabilities();
            std::cout << "Capabilities: Version " << std::showbase << std::hex << capabilities.Version << std::endl;
        } catch (const UwbException& e) {
            std::cerr << "failed to obtain uwb simulator capabilities (error=" << ::ToString(e.Status) << ")";
            return -3;
        }
    }

    std::cout << "initialization succeeded" << std::endl;

    return 0;
}
