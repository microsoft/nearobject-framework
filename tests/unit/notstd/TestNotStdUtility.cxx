
#include <array>
#include <cstdint>

#include <notstd/utility.hxx>

#include <catch2/catch_test_macros.hpp>

namespace notstd::test
{
static constexpr uint8_t ValueOne = 0xAA;
static constexpr uint8_t ValueTwo = 0xFF;

enum class UnderlyingByte : uint8_t {
    One = ValueOne,
    Two = ValueTwo,
};

constexpr UnderlyingByte ValueOneEnum = UnderlyingByte::One;
constexpr UnderlyingByte ValueTwoEnum = UnderlyingByte::Two;
} // namespace notstd::test

TEST_CASE("to_underlying works with uint8_t", "[basic]")
{
    using namespace notstd;
    using notstd::test::UnderlyingByte;

    constexpr auto underlyingValueOne = to_underlying(test::ValueOneEnum);
    constexpr auto underlyingValueTwo = to_underlying(test::ValueTwoEnum);

    REQUIRE(underlyingValueOne == test::ValueOne);
    REQUIRE(underlyingValueTwo == test::ValueTwo);
}
