
#include <filesystem>

#include <nearobject/persist/NearObjectProfilePersisterFilesystem.hxx>
#include <nearobject/service/NearObjectDeviceManager.hxx>
#include <nearobject/service/NearObjectService.hxx>
#include <nearobject/service/ServiceRuntime.hxx>
#include <windows/nearobject/service/NearObjectDeviceDiscoveryAgentUwb.hxx>

using namespace nearobject::persistence;
using namespace nearobject::service;
using windows::nearobject::service::NearObjectDeviceDiscoveryAgentUwb;

int
main(int argc, char *argv[])
{
    static const std::filesystem::path PersistencePath = "%USERPROFILE%/.nearobject";

    // Create profile manager.
    auto profilePersisterFs = std::make_unique<nearobject::persistence::NearObjectProfilePersisterFilesystem>(PersistencePath);
    auto profileManager = std::make_shared<NearObjectProfileManager>(std::move(profilePersisterFs));

    // Create device manager.
    auto uwbDeviceAgent = std::make_unique<NearObjectDeviceDiscoveryAgentUwb>();
    auto deviceManager = NearObjectDeviceManager::Create();
    deviceManager->AddDiscoveryAgent(std::move(uwbDeviceAgent));

    // Create service.
    auto service = NearObjectService::Create({
        std::move(profileManager),
        std::move(deviceManager)
    });

    // Start service runtime.
    ServiceRuntime nearObjectServiceRuntime{};
    nearObjectServiceRuntime.SetServiceInstance(service).Start();
    return 0;
}
