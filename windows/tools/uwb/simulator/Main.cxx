
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <tuple>

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

namespace detail
{
/**
 * @brief Decode a uwb simulator capabilities version into its component parts.
 *
 * @param capabilities The capabilities whose version is to be decoded.
 * @return std::tuple<uint16_t, uint16_t> The major and minor version components, in that order.
 */
std::tuple<uint16_t, uint16_t>
decodeSimulatorCapabilitiesVersion(const UwbSimulatorCapabilities& capabilities)
{
    static constexpr uint32_t VersionMajorMask = 0xFFFF0000U;
    static constexpr uint32_t VersionMinorMask = 0x0000FFFFU;
    static constexpr auto VersionMajorShift = 16U;
    static constexpr auto VersionMinorShift = 0U;

    const auto& version = capabilities.Version;
    const auto versionMajor = static_cast<uint16_t>((version & VersionMajorMask) >> VersionMajorShift);
    const auto versionMinor = static_cast<uint16_t>((version & VersionMinorMask) >> VersionMinorShift);
    return std::make_tuple(versionMajor, versionMinor);
}
} // namespace detail

int
main(int argc, char* argv[])
{
    plog::RollingFileAppender<plog::TxtFormatter> rollingFileAppender(logging::GetLogName("uwbsim").c_str());
    plog::DebugOutputAppender<plog::TxtFormatter> debugAppender;
    plog::init(plog::verbose, &rollingFileAppender).addAppender(&debugAppender);

    CLI::App app{};
    app.name("uwbsim");
    app.description("control and interact with uwb simulator devices");

    uint32_t sessionId;
    bool getCapabilities{ false };
    std::string deviceName{};
    auto deviceNameOpt = app.add_option("--deviceName, -d", deviceName, "The uwb simulator device name (path)");
    auto sessionIdOpt = app.add_option("--sessionId", sessionId, "session id for operations that require one");
    app.add_flag("--getCapabilities", getCapabilities, "enumerate the capabilities of the simulator");

    const std::map<std::string, UwbSimulatorSessionEventAction> eventActionMap{
        { "startrandomgeneration", UwbSimulatorSessionEventAction::RandomRangingMeasurementGenerationStart },
        { "stoprandomgeneration", UwbSimulatorSessionEventAction::RandomRangingMeasurementGenerationStop }
    };

    UwbSimulatorTriggerSessionEventArgs uwbSimulatorTriggerSessionEventArgs{};
    auto appTrigger = app.add_subcommand("trigger", "commands to trigger events on a session")->fallthrough();
    appTrigger->add_option("--eventAction, -e", uwbSimulatorTriggerSessionEventArgs.Action, "event action name")->required()->transform(CLI::CheckedTransformer(eventActionMap, CLI::ignore_case));
    appTrigger->needs(sessionIdOpt);

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

    std::cout << "initialization succeeded" << std::endl;

    if (getCapabilities) {
        try {
            auto capabilities = uwbDeviceSimulator->GetSimulatorCapabilities();
            auto [versionMajor, versionMinor] = detail::decodeSimulatorCapabilitiesVersion(capabilities);
            std::cout << "Capabilities: Version v" << versionMajor << '.' << versionMinor << std::endl;
        } catch (const UwbException& e) {
            std::cerr << "failed to obtain uwb simulator capabilities (error=" << ::ToString(e.Status) << ")";
            return -3;
        }
    }

    if (appTrigger->parsed()) {
        try {
            auto result = uwbDeviceSimulator->TriggerSessionEvent(uwbSimulatorTriggerSessionEventArgs);
        } catch (UwbException& e) {
            std::cerr << "failed to trigger session event (error=" << ::ToString(e.Status) << ")";
        }
    }

    return 0;
}
