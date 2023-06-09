
#include <catch2/catch_test_macros.hpp>
#include <magic_enum.hpp>
#include <nlohmann/json.hpp>
#include <uwb/UwbJsonSerializers.hxx>

TEST_CASE("UwbSessionEndReason enumeration serialization is stable", "[basic][uwb][serialize]")
{
    using namespace uwb;

    SECTION("serialization is stable")
    {
        for (const auto uwbSessionEndReason : magic_enum::enum_values<UwbSessionEndReason>()) {
            const auto json = nlohmann::json(uwbSessionEndReason);
            const auto uwbSessionEndReasonDeserialized = json.get<UwbSessionEndReason>();
            REQUIRE(uwbSessionEndReason == uwbSessionEndReasonDeserialized);
        }
    }
}
