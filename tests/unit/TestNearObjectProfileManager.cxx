
#include <filesystem>
#include <vector>

#include <catch2/catch.hpp>

#include <nearobject/NearObjectProfile.hxx>
#include <nearobject/service/NearObjectProfileManager.hxx>

namespace nearobject
{
namespace service
{
namespace test
{
class TestNearObjectProfileManager 
    : public NearObjectProfileManager
{
public:
    persist::PersistResult
    TestPersistProfile(const NearObjectProfile& profile)
    {
        return NearObjectProfileManager::PersistProfile(profile);
    }

    std::vector<NearObjectProfile>
    TestReadPersistedProfiles(persist::PersistResult& persistResult) const
    {
        return NearObjectProfileManager::ReadPersistedProfiles(persistResult);
    }
};
} // namespace test
} // namespace service
} // namespace nearobject

TEST_CASE("near object profile manager can be created", "[basic][service]")
{
    using namespace nearobject::service;

    SECTION("creation doesn't cause a crash")
    {
        NearObjectProfileManager profileManager{};
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

    SECTION("NearObjectProfileSecurity can be serialized and parsed")
    {
        using namespace nearobject;

        NearObjectProfileSecurity security1;
        NearObjectProfileSecurity security2;

        // rapidjson::Document document;
        // auto& allocator = document.GetAllocator();

        // const auto jsonValue = security1.ToJson(allocator);
        // const auto parseResult = security2.ParseAndSet(jsonValue);
        // REQUIRE(security1 == security2);
        // REQUIRE(parseResult == persist::ParseResult::Succeeded);
    }

    SECTION("NearObjectProfile (with no Security) can be serialized and parsed")
    {
        using namespace nearobject;

        NearObjectProfile profile1;
        NearObjectProfile profile2;

        // rapidjson::Document document;
        // auto& allocator = document.GetAllocator();

        // auto jsonValue = profile1.ToJson(allocator);
        // auto parseResult = profile2.ParseAndSet(jsonValue);
        // REQUIRE(parseResult == persist::ParseResult::Succeeded);
        // REQUIRE(profile1 == profile2);
    }

    SECTION("NearObjectProfile (with Security) can be serialized and parsed")
    {
        using namespace nearobject;
        using namespace nearobject::service;

        NearObjectProfile profile1;
        NearObjectProfile profile2;
        NearObjectProfileSecurity security;

        profile1.Security.emplace(std::move(security));

        // rapidjson::Document document;
        // auto& allocator = document.GetAllocator();

        // auto jsonValue = profile1.ToJson(allocator);
        // auto parseResult = profile2.ParseAndSet(jsonValue);
        // REQUIRE(parseResult == persist::ParseResult::Succeeded);
        // REQUIRE(profile1 == profile2);
    }

    SECTION("NearObjectProfileManager::PersistProfile matches the read profiles")
    {
        using namespace nearobject;
        using namespace nearobject::service;

        NearObjectProfile profile1;
        NearObjectProfile profile2;
        NearObjectProfileSecurity security;

        profile1.Security.emplace(std::move(security));

        // rapidjson::Document document;
        // auto& allocator = document.GetAllocator();

        // const auto persistLocation = testTempDirectory / std::filesystem::path("profiles");

        // // remove the file
        // try {
        //     std::filesystem::remove(persistLocation);
        // } catch (const std::filesystem::filesystem_error&) {
        // }

        // test::TestNearObjectProfileManager profileManager{};
        // profileManager.SetPersistLocation(persistLocation);

        // // persist the profiles
        // auto result = profileManager.TestPersistProfile(profile1);
        // REQUIRE(result == persist::PersistResult::Succeeded);
        // result = profileManager.TestPersistProfile(profile2);
        // REQUIRE(result == persist::PersistResult::Succeeded);

        // // read the profiles
        // persist::PersistResult persistResult = persist::PersistResult::UnknownError;
        // auto profiles = profileManager.TestReadPersistedProfiles(persistResult);

        // // remove the file
        // try {
        //     std::filesystem::remove(persistLocation);
        // } catch (const std::filesystem::filesystem_error&) {
        // }

        // REQUIRE(persistResult == persist::PersistResult::Succeeded);
        // REQUIRE(profiles.size() == 2);
        // REQUIRE((profile1 == profiles[0] || profile1 == profiles[1]));
        // if (profile1 == profiles[0]) {
        //     REQUIRE(profile2 == profiles[1]);
        // } else {
        //     REQUIRE(profile2 == profiles[0]);
        // }
    }
    std::filesystem::remove(testTempDirectory);
}
