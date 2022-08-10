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
    void
    TestPersistProfile(const NearObjectProfile& profile)
    {
        NearObjectProfileManager::PersistProfile(profile);
    };
    std::vector<NearObjectProfile>
    TestReadPersistedProfiles() const
    {
        return NearObjectProfileManager::ReadPersistedProfiles();
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
    SECTION("NearObjectProfileSecurity can be serialized and parsed")
    {
        nearobject::NearObjectConnectionProfileSecurity Sec1, Sec2;
        rapidjson::Document doc;

        auto& allocator = doc.GetAllocator();

        auto v = Sec1.to_json(allocator); // TOOD verify that to_json succeeded
        auto presult = Sec2.parse_and_set(v);
        REQUIRE(nearobject::NearObjectConnectionProfileSecurity::profiles_match(Sec1, Sec2));
        REQUIRE(presult == persist::ParseResult::Succeeded);
    }

    SECTION("NearObjectProfile (with no Security) can be serialized and parsed")
    {
        nearobject::NearObjectProfile profile, profile2;
        rapidjson::Document doc;

        auto& allocator = doc.GetAllocator();

        auto v = profile.to_json(allocator); // TOOD verify that to_json succeeded
        auto presult = profile2.parse_and_set(v);
        REQUIRE(presult == persist::ParseResult::Succeeded);
        REQUIRE(nearobject::NearObjectProfile::profiles_match(profile, profile2));
    }

    SECTION("NearObjectProfile (with Security) can be serialized and parsed")
    {
        rapidjson::Document doc;

        nearobject::NearObjectProfile profile, profile2;
        nearobject::NearObjectConnectionProfileSecurity Sec;

        *profile.Security = Sec;

        auto& allocator = doc.GetAllocator();

        auto v = profile.to_json(allocator); // TOOD verify that to_json succeeded
        auto presult = profile2.parse_and_set(v);
        REQUIRE(presult == persist::ParseResult::Succeeded);
        REQUIRE(nearobject::NearObjectProfile::profiles_match(profile, profile2));
    }

    SECTION("NearObjectProfileManager::PersistProfile matches the read profiles")
    {
        rapidjson::Document doc;

        nearobject::NearObjectProfile profile, profile2;
        nearobject::NearObjectConnectionProfileSecurity Sec;

        *profile.Security = Sec;

        auto& allocator = doc.GetAllocator();

        auto persist_location = "testprofiles";

        // remove the file
        try {
            std::filesystem::remove(persist_location);
        } catch (const std::filesystem::filesystem_error& err) {
        }

        nearobject::service::TestNearObjectProfileManager profileManager{};
        profileManager.SetPersistLocation(persist_location);

        // persist the profiles
        profileManager.TestPersistProfile(profile);
        profileManager.TestPersistProfile(profile2);

        // read the profiles
        auto profiles = profileManager.TestReadPersistedProfiles();

        // remove the file
        try {
            std::filesystem::remove(persist_location);
        } catch (const std::filesystem::filesystem_error& err) {
        }

        REQUIRE(profiles.size() == 2);
        REQUIRE((nearobject::NearObjectProfile::profiles_match(profile, profiles[0]) ||
            nearobject::NearObjectProfile::profiles_match(profile, profiles[1])));
        REQUIRE((nearobject::NearObjectProfile::profiles_match(profile2, profiles[0]) ||
            nearobject::NearObjectProfile::profiles_match(profile2, profiles[1])));
    }
}
