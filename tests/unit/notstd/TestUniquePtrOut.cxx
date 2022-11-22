
#include <memory>

#include <notstd/unique_ptr_out.hxx>

#include <catch2/catch_test_macros.hpp>

namespace test
{
struct Thing
{};
struct ThingFactory
{
    Thing*
    CreateThing(Thing* thingIn, Thing** thingOut)
    {
        *thingOut = thingIn;
        return thingIn;
    }

    Thing*
    CreateThingNew(Thing** thingNew)
    {
        return CreateThing(std::make_unique<Thing>().release(), thingNew);
    }

    Thing*
    CreateThingNull(Thing** thingNull)
    {
        return CreateThing(nullptr, thingNull);
    }
};
} // namespace test

TEST_CASE("output pointer works", "[basic]")
{
    test::ThingFactory thingFactory{};

    SECTION("valid pointer value is proxied")
    {
        std::unique_ptr<test::Thing> thing;
        const auto thingExpected = thingFactory.CreateThingNew(notstd::unique_ptr_out(thing));
        REQUIRE(thing.get() == thingExpected);
    }

    SECTION("null pointer value is proxied")
    {
        std::unique_ptr<test::Thing> thing;
        const auto thingExpected = thingFactory.CreateThingNull(notstd::unique_ptr_out(thing));
        REQUIRE(thing.get() == thingExpected);
    }
}
