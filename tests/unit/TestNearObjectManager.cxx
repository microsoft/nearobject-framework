
#include <array>
#include <memory>

#include <catch2/catch_test_macros.hpp>

#include <nearobject/NearObjectManager.hxx>

TEST_CASE("near object manager can be created", "[basic]")
{
    using namespace nearobject;

    SECTION("creation doesn't cause a crash")
    {
        NearObjectManager manager{};
    }
}

TEST_CASE("near object manager apis don't cause a crash")
{
    using namespace nearobject;

    SECTION("FindAllConnectionProfiles doesn't cause a crash")
    {
        NearObjectManager manager{};
        const auto connectionProfiles = manager.FindAllConnectionProfiles();
    }

    SECTION("CreateSession doesn't cause a crash with unknown profile")
    {
        NearObjectManager manager{};
        const NearObjectProfile connectionProfile{};
        REQUIRE_NOTHROW(manager.CreateSession(connectionProfile));
    }

    SECTION("CreateSession doesn't cause a crash with any known profile")
    {
        NearObjectManager manager{};
        const auto connectionProfiles = manager.FindAllConnectionProfiles();
        for (const auto& connectionProfile : connectionProfiles) {
            REQUIRE_NOTHROW(manager.CreateSession(connectionProfile));
        }
    }
}
