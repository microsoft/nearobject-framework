
#include <cerrno>
#include <stdexcept>
#include <string>

#include <unistd.h>

#include <nearobject/persist/NearObjectProfilePersisterFilesystem.hxx>
#include <nearobject/service/NearObjectDeviceControllerManager.hxx>
#include <nearobject/service/NearObjectService.hxx>
#include <nearobject/service/NearObjectServiceConfiguration.hxx>
#include <nearobject/service/NearObjectServiceInjector.hxx>
#include <nearobject/service/ServiceRuntime.hxx>

#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Log.h>

#include <LogUtils/LogUtils.hxx>

/**
 * @brief Get the user's home path.
 * 
 * @return std::filesystem::path Returns a valid path if it could be resolved,
 * otherwise an empty path is returned indicating it could not be resolved.
 */
std::filesystem::path
GetUserHomePath() noexcept
{
    static constexpr auto LinuxHomePathEnvironmentValueName = "HOME";

    // NOLINTNEXTLINE(concurrency-mt-unsafe)
    char *value = std::getenv(LinuxHomePathEnvironmentValueName);
    if (!value) {
        return {};
    }

    return value;
}

using namespace nearobject::persistence;
using namespace nearobject::service;

int
main(int argc, char *argv[])
{
    plog::init(plog::verbose, logging::GetLogName("nearobjectd").c_str());

    auto configuration = nearobject::service::NearObjectServiceConfiguration::FromCommandLineArguments(argc, argv);

    // Resolve user home directory.
    const std::filesystem::path homePath{ GetUserHomePath() };
    if (homePath.empty()) {
        PLOG_FATAL << "unable to determine user home path";
        throw std::runtime_error("unable to determine user home path");
    }

    const std::filesystem::path persistencePath = homePath / nearobject::persistence::PathSuffix;

    // Create profile manager.
    auto profilePersisterFs = std::make_unique<nearobject::persistence::NearObjectProfilePersisterFilesystem>(persistencePath);
    auto profileManager = std::make_shared<NearObjectProfileManager>(std::move(profilePersisterFs));

    // Create device manager.
    auto deviceManager = NearObjectDeviceControllerManager::Create();
    // TODO: add discovery agent(s)

    // Create service.
    auto service = NearObjectService::Create({ std::move(profileManager),
        std::move(deviceManager), nullptr });

    // Start service runtime.
    ServiceRuntime nearObjectServiceRuntime{};
    nearObjectServiceRuntime.SetServiceInstance(service).Start();

    // Daemonize, if requested.
    if (configuration.RunInBackground) {
        constexpr int nochdir = 0; // Change current working directory to /
        constexpr int noclose = 0; // Don't redirect stdin, stdout to /dev/null
        if (daemon(nochdir, noclose) != 0) {
            int error = errno;
            const std::string what = "failed to daemonize (error=" + std::to_string(error) + ")";
            PLOG_FATAL << what;
            throw std::runtime_error(what);
        }
    }

    return 0;
}
