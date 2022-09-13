
#include <memory>

#include <catch2/catch.hpp>
#include <nearobject/service/NearObjectDeviceControllerManager.hxx>

TEST_CASE("near object device manager can be created", "[basic][service]")
{
    using namespace nearobject::service;

    SECTION("device manager doesn't cause a crash when created")
    {
        auto deviceManager = NearObjectDeviceControllerManager::Create();
    }
}
