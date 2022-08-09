
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

#include <filesystem>

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
    
    SECTION("NearObjectProfile (with Security) can be serialized and parsed")
    {
        rapidjson::Document doc;
        
        nearobject::NearObjectProfile profile, profile2;
        nearobject::NearObjectConnectionProfileSecurity Sec;

        *profile.Security = Sec;

        auto& allocator = doc.GetAllocator();

        auto v = profile.to_serial(allocator); // TOOD verify that to_serial succeeded
        auto presult = profile2.parse_and_set(v);
        REQUIRE(presult == persist::ParseResult::Succeeded);
        REQUIRE(nearobject::NearObjectProfile::profiles_match(profile,profile2));
    }

    SECTION("NearObjectProfileManager::PersistProfile matches the read profiles")
    {
        rapidjson::Document doc;
        
        nearobject::NearObjectProfile profile, profile2;
        nearobject::NearObjectConnectionProfileSecurity Sec;

        *profile.Security = Sec;

        auto& allocator = doc.GetAllocator();

        auto persist_location = "/home/shaneguan/nearobject-framework/build/testprofiles";

        // remove the file
        try {
            std::filesystem::remove(persist_location);
        }
        catch(const std::filesystem::filesystem_error& err) {
        }

        nearobject::service::NearObjectProfileManager profileManager{};
        profileManager.SetPersistLocation(persist_location);
        
        // persist the profiles
        profileManager.PersistProfile(profile);
        profileManager.PersistProfile(profile2);

        // read the profiles
        auto profiles = profileManager.ReadPersistedProfiles();

        // remove the file
        try {
            std::filesystem::remove(persist_location);
        }
        catch(const std::filesystem::filesystem_error& err) {
        }
        
        REQUIRE(profiles.size()==2);
        REQUIRE((nearobject::NearObjectProfile::profiles_match(profile,profiles[0]) || 
                nearobject::NearObjectProfile::profiles_match(profile,profiles[1])));
        REQUIRE((nearobject::NearObjectProfile::profiles_match(profile2,profiles[0]) || 
                nearobject::NearObjectProfile::profiles_match(profile2,profiles[1])));
    }
    
}

