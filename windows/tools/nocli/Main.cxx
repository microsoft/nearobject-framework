
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>

#include <CLI/CLI.hpp>

#include "NearObjectCliDataWindows.hxx"
#include "NearObjectCliHandlerWindows.hxx"

#include <nearobject/cli/NearObjectCli.hxx>
#include <nearobject/cli/NearObjectCliUwbSessionEventCallbacks.hxx>
#include <windows/devices/DeviceEnumerator.hxx>
#include <windows/devices/DevicePresenceMonitor.hxx>
#include <windows/devices/uwb/UwbDevice.hxx>

#include <plog/Appenders/RollingFileAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Init.h>
#include <plog/Log.h>

#include <logging/LogUtils.hxx>
#include <logging/windows/PlogDebugWrapper.hxx>

int
main(int argc, char* argv[])
try {
    plog::RollingFileAppender<plog::TxtFormatter> rollingFile(logging::GetLogName("nocli").c_str());
    logging::plog::DebugWrapperAppender<plog::TxtFormatter> finalAppender(&rollingFile);
    plog::init(plog::verbose, &finalAppender);

    auto cliData = std::make_shared<nearobject::cli::NearObjectCliDataWindows>();
    auto cliHandler = std::make_shared<nearobject::cli::NearObjectCliHandlerWindows>();

    nearobject::cli::NearObjectCli cli{ cliData, cliHandler };

    // Configure the cli parsing app with Windows-specific options.
    auto& uwbApp = cli.GetUwbApp();
    uwbApp.add_option("--deviceName", cliData->DeviceName, "uwb device name (path)");
    uwbApp.add_option("--deviceClass", cliData->DeviceClassGuid, "uwb device class guid (override)");
    uwbApp.add_flag("--probe", cliData->DeviceNameProbe, "probe for the uwb device name to use");

    // Parse the arguments.
    int result = cli.Parse(argc, argv);
    if (result != 0) {
        return result;
    }

    return 0;
} catch (...) {
    // TODO
}
