
#include <array>
#include <memory>

#include <catch2/catch_test_macros.hpp>
#include <nearobject/service/NearObjectService.hxx>

TEST_CASE("near object service can be created", "[basic][service]")
{
    using namespace nearobject::service;

    SECTION("service doesn't cause a crash when created with default-initialized injector")
    {
        auto service = NearObjectService::Create({});
    }

    SECTION("service doesn't case a crash when created with missing components")
    {
        {   // missing profile manager
            auto service = NearObjectService::Create({
                nullptr,
                NearObjectDeviceControllerManager::Create(),
            });
        }
        {
            // missing device manager
            auto service = NearObjectService::Create({
                std::make_shared<NearObjectProfileManager>(),
                nullptr,
            });
        }
        {
            // missing both profile manager and device manager
            // missing device manager
            auto service = NearObjectService::Create({
                nullptr,
                nullptr,
            });
        }
    }

    SECTION("service exposes same components as provided by the injector")
    {
        std::shared_ptr<NearObjectProfileManager> profileManager;
        std::shared_ptr<NearObjectDeviceControllerManager> deviceManager;

        {
            profileManager = nullptr;
            deviceManager = nullptr;

            auto service = NearObjectService::Create({ profileManager, deviceManager });
            REQUIRE(service->ProfileManager == profileManager);
            REQUIRE(service->DeviceManager == deviceManager);
        }
        {
            profileManager = std::make_shared<NearObjectProfileManager>();
            deviceManager = NearObjectDeviceControllerManager::Create();

            auto service = NearObjectService::Create({ profileManager, deviceManager });
            REQUIRE(service->ProfileManager == profileManager);
            REQUIRE(service->DeviceManager == deviceManager);
        }
    }
}
