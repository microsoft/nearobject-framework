
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>

#include <CLI/CLI.hpp>

#include <windows/devices/uwb/UwbDevice.hxx>

#include "NearObjectCliDataWindows.hxx"
#include "NearObjectCliHandlerWindows.hxx"

#include <nearobject/cli/NearObjectCli.hxx>
#include <nearobject/cli/NearObjectCliUwbSessionEventCallbacks.hxx>

#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Appenders/DebugOutputAppender.h>
#include <plog/Appenders/RollingFileAppender.h>
#include <plog/Formatters/MessageOnlyFormatter.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Init.h>
#include <plog/Log.h>

#include <logging/LogUtils.hxx>

#include <winrt/windows.devices.nearobject.h>

int
main(int argc, char* argv[])
try {
    plog::RollingFileAppender<plog::TxtFormatter> rollingFileAppender(logging::GetLogName("nocli").c_str());
    plog::DebugOutputAppender<plog::TxtFormatter> debugAppender;
    plog::ColorConsoleAppender<plog::MessageOnlyFormatter> colorConsoleAppender{};
    plog::init(plog::verbose, &rollingFileAppender)
        .addAppender(&debugAppender)
        .addAppender(&colorConsoleAppender);

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

    // Wait for all requested operations to complete running.
    cli.WaitForExecutionComplete();

    return 0;
} catch (...) {
    // TODO
}
