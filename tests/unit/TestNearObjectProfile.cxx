
#include <catch2/catch.hpp>

#include <nearobject/NearObjectProfile.hxx>

TEST_CASE("near object profile equality operator works", "[basic]")
{
    using namespace nearobject;

    SECTION("returns true for instances without security")
    {
        NearObjectProfile profile1{};
        NearObjectProfile profile2{};
        REQUIRE(profile1 == profile2);
    }

    SECTION("returns false for instances with differing security configuration")
    {
        NearObjectProfile profile1{ NearObjectConnectionScope::Unicast};
        NearObjectProfile profile2{ NearObjectConnectionScope::Unicast, NearObjectProfileSecurity{} };
        REQUIRE(profile1 != profile2);
    }

    SECTION("returns true for instances with matching security")
    {
        NearObjectProfile profile1{ NearObjectConnectionScope::Unicast, NearObjectProfileSecurity{} };
        NearObjectProfile profile2{ NearObjectConnectionScope::Unicast, NearObjectProfileSecurity{} };
        REQUIRE(profile1 == profile2);
    }
}
