
#include <catch2/catch_test_macros.hpp>

#include <memory>

#include <uwb/protocols/fira/UwbApplicationConfiguration.hxx>

TEST_CASE("uwb application configuration objects can be created", "[basic]")
{
    using namespace uwb::protocol::fira;

    SECTION("creation doesn't cause a crash")
    {
        REQUIRE_NOTHROW(std::make_unique<UwbApplicationConfiguration>());
    }

    SECTION("destruction doesn't cause a crash")
    {
        auto uwbApplicationConfiguration = std::make_unique<UwbApplicationConfiguration>();
        REQUIRE_NOTHROW([&] {
            uwbApplicationConfiguration.reset();
        });
    }
}
