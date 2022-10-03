
#include <array>
#include <cstdint>

#include <catch2/catch.hpp>

#include <notstd/hash.hxx>

TEST_CASE("hash_range is not sensitive to ordering", "[basic][hash][order]")
{
    std::array<uint8_t, 4> arrayOne{0,1,2,3};
    std::array<uint8_t, 4> arrayTwo{3,2,1,0};
    auto hashOne = notstd::hash_range(std::cbegin(arrayOne), std::cend(arrayOne));
    auto hashTwo = notstd::hash_range(std::cbegin(arrayTwo), std::cend(arrayTwo));
    REQUIRE(hashOne != hashTwo);
}
