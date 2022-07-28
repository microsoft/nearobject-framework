
#include <array>
#include <memory>

#include <catch2/catch.hpp>
#include <nearobject/service/NearObjectService.hxx>

TEST_CASE("near object service can be created", "[basic][service]")
{
    using namespace nearobject::service;

    SECTION("service doesn't cause a crash when created with empty injector")
    {
        NearObjectServiceInjector injector{};
        NearObjectService service{ std::move(injector) };
    }

    SECTION("service doesn't case a crash when created with missing connection profile manager")
    {
        NearObjectServiceInjector injector{
            nullptr,
            {}
        };
        NearObjectService service{ std::move(injector) };
    }

    SECTION("service doesn't cause a crash when created with single device manager")
    {
        NearObjectServiceInjector injector{};
        injector.ConnectionProfileManager = std::make_unique<NearObjectConnectionProfileManager>();
        injector.DeviceManagers.push_back(std::make_unique<NearObjectDeviceManager>());
        NearObjectService service{ std::move(injector) };
    }
}
