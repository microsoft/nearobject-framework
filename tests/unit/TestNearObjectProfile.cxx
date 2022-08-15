
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
        NearObjectProfile profile1{};
        NearObjectProfile profile2{};
        profile2.Security.emplace();
        REQUIRE(profile1 != profile2);
    }

    SECTION("returns true for instances with matching security")
    {
        NearObjectProfile profile1{};
        NearObjectProfile profile2{};
        profile1.Security.emplace();
        profile2.Security.emplace();
        REQUIRE(profile1 == profile2);
    }

    SECTION("result changes when equality changes")
    {
        NearObjectProfile profile1{ NearObjectConnectionScope::Unicast };
        NearObjectProfile profile2{ NearObjectConnectionScope::Unicast };
        REQUIRE(profile1 == profile2);

        // Change security configuration.
        profile1.Security.emplace();
        REQUIRE(profile1 != profile2);

        // Reset security, verify equal.
        profile1.Security.reset();
        REQUIRE(profile1 == profile2);

        // Change scope.
        profile2.Scope = NearObjectConnectionScope::Multicast;
        REQUIRE(profile1 != profile2);
    }
}
