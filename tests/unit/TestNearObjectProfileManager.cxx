
#include <filesystem>
#include <vector>

#include <catch2/catch.hpp>

#include <nearobject/NearObjectProfile.hxx>
#include <nearobject/service/NearObjectProfileManager.hxx>
#include <nearobject/persist/NearObjectProfilePersisterFilesystem.hxx>

TEST_CASE("near object profile manager can be created", "[basic][service]")
{
    using namespace nearobject::persistence;
    using namespace nearobject::service;

    SECTION("creation doesn't cause a crash")
    {
        NearObjectProfileManager profileManager{};
    }

    SECTION("creation with custom persister doesn't cause a crash")
    {
        auto persisterFs = std::make_unique<NearObjectProfilePersisterFilesystem>();
        REQUIRE_NOTHROW(std::make_unique<NearObjectProfileManager>(std::move(persisterFs))); 
    }

    SECTION("creation with invalid persister causes a crash")
    {
        REQUIRE_THROWS_AS(std::make_unique<NearObjectProfileManager>(nullptr), std::runtime_error);
    }
}

TEST_CASE("near object profiles can be enumerated")
{
    using namespace nearobject;
    using namespace nearobject::service;

    SECTION("enumerating all profiles doesn't cause a crash")
    {
        NearObjectProfileManager profileManager{};
        REQUIRE_NOTHROW(profileManager.GetAllProfiles());
    }

    SECTION("attempting to find a non-existent profile doesn't cause a crash")
    {
        NearObjectProfile profile{};
        NearObjectProfileManager profileManager{};
        std::vector<NearObjectProfile> profilesMatching{};

        REQUIRE_NOTHROW([&]() {
            profilesMatching = profileManager.FindMatchingProfiles(profile);
        }());
        REQUIRE(profilesMatching.empty());
    }
}

TEST_CASE("near object profiles can be persisted", "[basic][infra]")
{
}
