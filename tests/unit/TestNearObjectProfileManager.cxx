
#include <catch2/catch.hpp>

#include <nearobject/NearObjectProfile.hxx>
#include <nearobject/service/NearObjectProfileManager.hxx>

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

        auto v = Sec1.to_serial(allocator); // TOOD verify that to_serial succeeded
        auto presult = Sec2.parse_and_set(v);
        REQUIRE(nearobject::NearObjectConnectionProfileSecurity::profiles_match(Sec1,Sec2));
        REQUIRE(presult == persist::ParseResult::Succeeded);
    }
    
    SECTION("NearObjectProfile (with no Security) can be serialized and parsed")
    {
        nearobject::NearObjectProfile profile, profile2;
        rapidjson::Document doc;

        auto& allocator = doc.GetAllocator();

        auto v = profile.to_serial(allocator); // TOOD verify that to_serial succeeded
        auto presult = profile2.parse_and_set(v);
        REQUIRE(presult == persist::ParseResult::Succeeded);
        REQUIRE(nearobject::NearObjectProfile::profiles_match(profile,profile2));
    }
    
    SECTION("NearObjectProfileSecurity can be serialized and parsed")
    {
        nearobject::NearObjectConnectionProfileSecurity Sec;
        rapidjson::Document doc;

        auto& allocator = doc.GetAllocator();

        auto v = Sec.to_serial(allocator); // TOOD verify that to_serial succeeded
        auto presult = Sec.parse_and_set(v);
        REQUIRE(presult == persist::ParseResult::Succeeded);
    }
    
}

