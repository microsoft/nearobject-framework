
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
}
