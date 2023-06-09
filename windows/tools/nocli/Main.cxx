
#include <cstdint>
#include <functional>
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

/**
 * @brief Function that will be executed by the control sequence handler on a
 * stop request (eg. Ctrl-C, Ctrl-Break, etc.). Populated by the main() function
 * once parsing is complete
 */
static std::function<void(void)>
    OnApplicationStopRequest{ nullptr };

/**
 * @brief Control sequence handler.
 *
 * @param controlType The type of control signal sent.
 * @return BOOL Whether the control signal was handled.
 */
BOOL WINAPI
CtrlHandler(DWORD controlType)
{
    switch (controlType) {
    case CTRL_C_EVENT:
    case CTRL_CLOSE_EVENT:
    case CTRL_BREAK_EVENT:
    case CTRL_LOGOFF_EVENT:
    case CTRL_SHUTDOWN_EVENT:
        if (OnApplicationStopRequest != nullptr) {
            OnApplicationStopRequest();
        }
        return TRUE;
    }

    return FALSE;
}

/**
 * @brief Instance IDs for PLOG loggers
 */
enum class LogInstanceId {
    // Default logger is 0 and is omitted from this enum
    Console = 1,
    File = 2,
};

int
main(int argc, char* argv[])
try {
    static plog::ColorConsoleAppender<plog::MessageOnlyFormatter> colorConsoleAppender{};
    plog::init<static_cast<int>(LogInstanceId::Console)>(plog::info, &colorConsoleAppender);

    static plog::RollingFileAppender<plog::TxtFormatter> rollingFileAppender(logging::GetLogName("nocli").c_str());
    plog::init<static_cast<int>(LogInstanceId::File)>(plog::verbose, &rollingFileAppender);

    plog::init(plog::verbose).addAppender(plog::get<static_cast<int>(LogInstanceId::Console)>()).addAppender(plog::get<static_cast<int>(LogInstanceId::File)>());

    auto cliData = std::make_shared<nearobject::cli::NearObjectCliDataWindows>();
    auto cliHandler = std::make_shared<nearobject::cli::NearObjectCliHandlerWindows>();

    nearobject::cli::NearObjectCli cli{ cliData, cliHandler };

    // Configure stop handler to cancel and wait for execution to complete.
    OnApplicationStopRequest = [&] {
        PLOG_VERBOSE << "handling stop request sent via control signal";
        cli.CancelExecution();
        cli.WaitForExecutionComplete();
        PLOG_INFO << "exiting";
    };

    // Register console control signal handler.
    {
        static constexpr auto AddHandler = TRUE;
        SetConsoleCtrlHandler(CtrlHandler, AddHandler);
    }

    // Configure the cli parsing app with Windows-specific options.
    auto& uwbApp = cli.GetDriverUwbApp();
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
