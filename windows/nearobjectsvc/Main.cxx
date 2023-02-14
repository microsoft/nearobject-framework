
#include <cstdlib>
#include <filesystem>
#include <memory>
#include <stdexcept>

#include <nearobject/persist/NearObjectProfilePersisterFilesystem.hxx>
#include <nearobject/service/NearObjectDeviceControllerManager.hxx>
#include <nearobject/service/NearObjectService.hxx>
#include <nearobject/service/ServiceRuntime.hxx>
#include <windows/nearobject/service/NearObjectDeviceDiscoveryAgentUwb.hxx>

#include <plog/Formatters/TxtFormatter.h>
#include <plog/Init.h>
#include <plog/Log.h>

#include <logging/windows/PlogDebugWrapper.hxx>
#include <logging/windows/PlogTraceLogging.hxx>

TRACELOGGING_DEFINE_PROVIDER(
    NearObjectSvcTraceLoggingProvider,
    "NearObjectSvcTraceLoggingProvider",
    // {EB735AF1-3A63-4650-B4B0-47A4CEE75468}
    (0xeb735af1, 0x3a63, 0x4650, 0xb4, 0xb0, 0x47, 0xa4, 0xce, 0xe7, 0x54, 0x68));

/**
 * @brief Get the user's home path.
 *
 * @return std::filesystem::path Returns a valid path if it could be resolved,
 * otherwise an empty path is returned indicating it could not be resolved.
 */
std::filesystem::path
GetUserHomePath() noexcept
{
    static constexpr auto WindowsHomePathEnvironmentValueName = "HOMEPATH";

    std::size_t valueCount = 0;
    char *valueBuffer = nullptr;
    if (_dupenv_s(&valueBuffer, &valueCount, WindowsHomePathEnvironmentValueName) != 0) {
        return {};
    }

    // Assign ownership of the buffer allocated from _dupenv_s() to a smart
    // pointer to free it at the end of the function scope.
    std::unique_ptr<char, decltype(&std::free)> value{ valueBuffer, std::free };
    return valueBuffer;
}

using namespace nearobject::persistence;
using namespace nearobject::service;
using windows::nearobject::service::NearObjectDeviceDiscoveryAgentUwb;

int
main(int argc, char *argv[])
{
    logging::plog::TraceLoggingAppender<plog::TxtFormatter> traceLoggingAppender(NearObjectSvcTraceLoggingProvider);
    logging::plog::DebugWrapperAppender<plog::TxtFormatter> finalAppender(&traceLoggingAppender);
    plog::init(plog::debug, &finalAppender);

    // Resolve user home directory.
    const std::filesystem::path homePath{ GetUserHomePath() };
    if (homePath.empty()) {
        throw std::runtime_error("unable to determine user home path");
    }

    const std::filesystem::path persistencePath = homePath / nearobject::persistence::PathSuffix;

    // Create profile manager.
    auto profilePersisterFs = std::make_unique<nearobject::persistence::NearObjectProfilePersisterFilesystem>(persistencePath);
    auto profileManager = std::make_shared<NearObjectProfileManager>(std::move(profilePersisterFs));

    // Create device manager.
    auto uwbDeviceAgent = std::make_unique<NearObjectDeviceDiscoveryAgentUwb>();
    auto deviceManager = NearObjectDeviceControllerManager::Create();
    deviceManager->AddDiscoveryAgent(std::move(uwbDeviceAgent));

    // Create service.
    auto service = NearObjectService::Create({ std::move(profileManager),
        std::move(deviceManager) });

    // Start service runtime.
    ServiceRuntime nearObjectServiceRuntime{};
    nearObjectServiceRuntime.SetServiceInstance(service).Start();
    return 0;
}
