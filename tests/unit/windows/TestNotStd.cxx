
#include <algorithm>
#include <array>
#include <functional>
#include <map>
#include <queue>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <catch2/catch.hpp>

#include <notstd/guid.hxx>

#include <initguid.h>

namespace notstd
{
namespace test
{
// 2fa25f41-e17f-41b9-b0fe-6581aef56ba8
DEFINE_GUID(GuidOne,   0x2fa25f41, 0xe17f, 0x41b9, 0xb0, 0xfe, 0x65, 0x81, 0xae, 0xf5, 0x6b, 0xa8);
// 3cf11b32-feef-4225-9b36-46331951befc
DEFINE_GUID(GuidTwo,   0x3cf11b32, 0xfeef, 0x4225, 0x9b, 0x36, 0x46, 0x33, 0x19, 0x51, 0xbe, 0xfc);
// 17fb06a9-1480-415c-883f-be4249e30043
DEFINE_GUID(GuidThree, 0x17fb06a9, 0x1480, 0x415c, 0x88, 0x3f, 0xbe, 0x42, 0x49, 0xe3, 0x00, 0x43);
} // namespace test
} // namespace notstd

TEST_CASE("GUID type can be used with STL containers", "[basic][shared][windows]")
{
    using namespace notstd;

    constexpr std::hash<GUID> guidHash{};
    constexpr std::equal_to<GUID> isGuidEqual{};

    SECTION("GUID can be compared for equality")
    {
        REQUIRE( isGuidEqual(test::GuidOne, test::GuidOne));
        REQUIRE(!isGuidEqual(test::GuidOne, test::GuidTwo));
    }

    SECTION("GUID can be used with std::hash")
    {
        REQUIRE(guidHash(test::GuidOne) == guidHash(test::GuidOne));
        REQUIRE(guidHash(test::GuidOne) != guidHash(test::GuidTwo));
    }

    SECTION("GUID can be used in sequence containers")
    {
        // declare built-in array
        const GUID builtInArrayOfGuids[3] = { test::GuidOne, test::GuidTwo, test::GuidThree };

        // array
        std::array<GUID, 2> arrayOfGuidsOne{ test::GuidOne, test::GuidTwo };
        std::array<GUID, 3> arrayOfGuidsTwo{ test::GuidOne, test::GuidTwo, test::GuidThree };
        REQUIRE( std::equal(std::cbegin(arrayOfGuidsOne), std::cend(arrayOfGuidsOne), std::cbegin(arrayOfGuidsOne)));
        REQUIRE(!std::equal(std::cbegin(arrayOfGuidsOne), std::cend(arrayOfGuidsOne), std::cbegin(arrayOfGuidsTwo), std::cend(arrayOfGuidsTwo)));

        // vector
        std::vector<GUID> vectorOfGuidsOne{ test::GuidOne, test::GuidTwo };
        std::vector<GUID> vectorOfGuidsTwo{ test::GuidOne, test::GuidTwo, test::GuidThree };
        REQUIRE(std::equal(std::cbegin(vectorOfGuidsOne), std::cend(vectorOfGuidsOne), std::cbegin(vectorOfGuidsOne)));
        REQUIRE(!std::equal(std::cbegin(vectorOfGuidsOne), std::cend(vectorOfGuidsOne), std::cbegin(vectorOfGuidsTwo), std::cend(vectorOfGuidsTwo)));
        vectorOfGuidsOne.push_back(test::GuidThree);
        REQUIRE(std::equal(std::cbegin(vectorOfGuidsOne), std::cend(vectorOfGuidsOne), std::cbegin(vectorOfGuidsTwo)));
        vectorOfGuidsOne.pop_back();
        REQUIRE(!std::equal(std::cbegin(vectorOfGuidsOne), std::cend(vectorOfGuidsOne), std::cbegin(vectorOfGuidsTwo), std::cend(vectorOfGuidsTwo)));
    }

    SECTION("GUID can be used in associative containers")
    {
        {   // set
            std::set<GUID> setOfGuidsOne{ test::GuidOne, test::GuidTwo };
            std::set<GUID> setOfGuidsTwo{ test::GuidOne, test::GuidTwo };
            REQUIRE(setOfGuidsOne == setOfGuidsTwo);

            // add element to make equal to setOfGuidsOne
            const auto [iterator, inserted] = setOfGuidsTwo.insert(test::GuidThree);
            REQUIRE(inserted == true);
            REQUIRE(*iterator == test::GuidThree);
            REQUIRE(setOfGuidsOne != setOfGuidsTwo);

            // remove added element
            const auto guidNodeOne = setOfGuidsTwo.extract(test::GuidThree);
            REQUIRE(!guidNodeOne.empty());
            REQUIRE(guidNodeOne.value() == test::GuidThree);
            REQUIRE(setOfGuidsOne == setOfGuidsTwo);

            // remove non-existent element
            const auto guidNodeTwo = setOfGuidsOne.extract(test::GuidThree);
            REQUIRE(guidNodeTwo.empty());

            // swap and verify equality
            setOfGuidsOne.swap(setOfGuidsTwo);
            REQUIRE(setOfGuidsOne == setOfGuidsTwo);
        }

        {   // map
            std::map<int, GUID> mapOfGuidsOne{ {1, test::GuidOne }, {2, test::GuidTwo } };
            std::map<int, GUID> mapOfGuidsTwo{ {1, test::GuidOne }, {2, test::GuidTwo } };
            REQUIRE(mapOfGuidsOne == mapOfGuidsTwo);

            // add element to make equal to mapOfGuidsOne
            const auto [iterator, inserted] = mapOfGuidsTwo.insert({3, test::GuidThree});
            REQUIRE(inserted == true);
            const auto [key, value] = *iterator;
            REQUIRE(key == 3);
            REQUIRE(value == test::GuidThree);
            REQUIRE(mapOfGuidsOne != mapOfGuidsTwo);

            // remove added element
            const auto guidNodeOne = mapOfGuidsTwo.extract(3);
            REQUIRE(!guidNodeOne.empty());
            REQUIRE(guidNodeOne.key() == 3);
            REQUIRE(guidNodeOne.mapped() == test::GuidThree);
            REQUIRE(mapOfGuidsOne == mapOfGuidsTwo);
        }

        // map, multi{set|map}
    }

    SECTION("GUID can be used in unordered associative containers")
    {
        // unordered_{set|map}, unordered_multi{set|map}
    }

    SECTION("GUID can be used in container adaptors")
    {
        // stack, queue, priority_queue
    }

    SECTION("GUID can be used with algorithms")
    {
        // std::transform, find_first_of, unordered_map?
    }
}
