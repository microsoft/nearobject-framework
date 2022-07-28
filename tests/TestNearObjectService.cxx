
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
        auto service = NearObjectService::Create(std::move(injector));
    }

    SECTION("service doesn't case a crash when created with missing profile manager")
    {
        NearObjectServiceInjector injector{
            nullptr,
            {}
        };
        auto service = NearObjectService::Create(std::move(injector));
    }

    SECTION("service doesn't cause a crash when created with single device manager")
    {
        NearObjectServiceInjector injector{};
        injector.ProfileManager = std::make_shared<NearObjectProfileManager>();
        injector.DeviceManager = std::make_shared<NearObjectDeviceManager>();
        auto service = NearObjectService::Create(std::move(injector));
    }
}
