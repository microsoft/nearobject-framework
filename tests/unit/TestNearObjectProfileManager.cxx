
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

    SECTION("FindMatchingProfiles doesn't cause a crash")
    {
        NearObjectProfile profile{};
        NearObjectProfileManager profileManager{};
        const auto matchingProfiles = profileManager.FindMatchingProfiles(profile);
    }
}

TEST_CASE("near object profiles can be persisted", "[basic][infra]")
{
    auto testTempDirectory = std::filesystem::path("NearObjectTestTemp");
    try {
        std::filesystem::remove(testTempDirectory);
    } catch (const std::filesystem::filesystem_error&) {
    }
    if (!std::filesystem::create_directories(testTempDirectory)) {
        throw std::filesystem::filesystem_error("could not create test directory", std::error_code());
    }

TEST_CASE("near object profiles can be persisted", "[basic][infra]")
{
}
