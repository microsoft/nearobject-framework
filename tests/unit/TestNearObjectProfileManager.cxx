#include <filesystem>

#include <catch2/catch.hpp>

#include <nearobject/NearObjectProfile.hxx>
#include <nearobject/service/NearObjectProfileManager.hxx>

namespace nearobject
{
namespace service
{
class TestNearObjectProfileManager : public NearObjectProfileManager
{
public:
    persist::PersistResult
    TestPersistProfile(const NearObjectProfile& profile)
    {
        return NearObjectProfileManager::PersistProfile(profile);
    };
    std::vector<NearObjectProfile>
    TestReadPersistedProfiles(persist::PersistResult& rcode) const
    {
        return NearObjectProfileManager::ReadPersistedProfiles(rcode);
    };
};
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

TEST_CASE("NearObjectProfile persistence", "[basic][infra]")
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
        nearobject::NearObjectConnectionProfileSecurity Sec1, Sec2;
        rapidjson::Document doc;

        auto& allocator = doc.GetAllocator();

        auto v = Sec1.to_json(allocator);
        auto presult = Sec2.ParseAndSet(v);
        REQUIRE(Sec1 == Sec2);
        REQUIRE(presult == persist::ParseResult::Succeeded);
    }

    SECTION("NearObjectProfile (with no Security) can be serialized and parsed")
    {
        nearobject::NearObjectProfile profile, profile2;
        rapidjson::Document doc;

        auto& allocator = doc.GetAllocator();

        auto v = profile.to_json(allocator);
        auto presult = profile2.ParseAndSet(v);
        REQUIRE(presult == persist::ParseResult::Succeeded);
        REQUIRE(profile == profile2);
    }

    SECTION("NearObjectProfile (with Security) can be serialized and parsed")
    {
        rapidjson::Document doc;

        nearobject::NearObjectProfile profile, profile2;
        nearobject::NearObjectConnectionProfileSecurity Sec;

        *profile.Security = Sec;

        auto& allocator = doc.GetAllocator();

        auto v = profile.to_json(allocator);
        auto presult = profile2.ParseAndSet(v);
        REQUIRE(presult == persist::ParseResult::Succeeded);
        REQUIRE(profile == profile2);
    }

    SECTION("NearObjectProfileManager::PersistProfile matches the read profiles")
    {
        rapidjson::Document doc;

        nearobject::NearObjectProfile profile, profile2;
        nearobject::NearObjectConnectionProfileSecurity Sec;

        *profile.Security = Sec;

        auto& allocator = doc.GetAllocator();

        auto persist_location = testTempDirectory / std::filesystem::path("profiles");

        // remove the file
        try {
            std::filesystem::remove(persist_location);
        } catch (const std::filesystem::filesystem_error&) {
        }

        nearobject::service::TestNearObjectProfileManager profileManager{};
        profileManager.SetPersistLocation(persist_location);

        // persist the profiles
        auto result = profileManager.TestPersistProfile(profile);
        REQUIRE(result == persist::PersistResult::Succeeded);
        result = profileManager.TestPersistProfile(profile2);
        REQUIRE(result == persist::PersistResult::Succeeded);

        // read the profiles
        persist::PersistResult rcode;
        auto profiles = profileManager.TestReadPersistedProfiles(rcode);

        // remove the file
        try {
            std::filesystem::remove(persist_location);
        } catch (const std::filesystem::filesystem_error&) {
        }

        REQUIRE(rcode == persist::PersistResult::Succeeded);
        REQUIRE(profiles.size() == 2);
        REQUIRE((profile == profiles[0] || profile == profiles[1]));
        if (profile == profiles[0]) {
            REQUIRE(profile2 == profiles[1]);
        } else {
            REQUIRE(profile2 == profiles[0]);
        }
    }
    std::filesystem::remove(testTempDirectory);
}
