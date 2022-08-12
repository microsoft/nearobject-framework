
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
DEFINE_GUID(Guid1, 0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01);
DEFINE_GUID(Guid2, 0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02);
DEFINE_GUID(Guid3, 0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03);

// Declare built-in array.
const std::array<GUID, 3> Guids = { Guid1, Guid2, Guid3 };

} // namespace test
} // namespace notstd

TEST_CASE("GUID type can be used with STL containers", "[basic][shared][windows]")
{
    using namespace notstd;

    constexpr std::hash<GUID> guidHash{};
    constexpr std::equal_to<GUID> isGuidEqual{};
    constexpr std::less<GUID> isGuidLess{};

    // Array of GUIDs with manually (lexicographically) ordered items.
    const std::array<GUID, 3> arrayOfOrderedGuids{ test::Guids[0], test::Guids[1], test::Guids[2] };

    SECTION("std::less<GUID> orders items correctly")
    {
        REQUIRE(!isGuidLess(arrayOfOrderedGuids[1], arrayOfOrderedGuids[0]));
        REQUIRE(!isGuidLess(arrayOfOrderedGuids[2], arrayOfOrderedGuids[0]));
        REQUIRE(!isGuidLess(arrayOfOrderedGuids[2], arrayOfOrderedGuids[1]));
        REQUIRE(!isGuidLess(arrayOfOrderedGuids[0], arrayOfOrderedGuids[0]));
        REQUIRE(!isGuidLess(arrayOfOrderedGuids[1], arrayOfOrderedGuids[1]));
        REQUIRE(!isGuidLess(arrayOfOrderedGuids[2], arrayOfOrderedGuids[2]));
    }

    SECTION("GUID can be compared for equality")
    {
        REQUIRE(isGuidEqual(test::Guids[0], test::Guids[0]));
        REQUIRE(!isGuidEqual(test::Guids[0], test::Guids[1]));
    }

    SECTION("GUID can be used with std::hash")
    {
        REQUIRE(guidHash(test::Guids[0]) == guidHash(test::Guids[0]));
        REQUIRE(guidHash(test::Guids[0]) != guidHash(test::Guids[1]));
        REQUIRE(guidHash(test::Guids[0]) != guidHash(test::Guids[2]));
        REQUIRE(guidHash(test::Guids[1]) != guidHash(test::Guids[2]));
    }

    SECTION("GUID can be used in sequence containers")
    {
        // array
        std::array<GUID, 2> arrayOfGuidsOne{ test::Guids[0], test::Guids[1] };
        std::array<GUID, 3> arrayOfGuidsTwo{ test::Guids[0], test::Guids[1], test::Guids[2] };
        REQUIRE(std::equal(std::cbegin(arrayOfGuidsOne), std::cend(arrayOfGuidsOne), std::cbegin(arrayOfGuidsOne)));
        REQUIRE(!std::equal(std::cbegin(arrayOfGuidsOne), std::cend(arrayOfGuidsOne), std::cbegin(arrayOfGuidsTwo), std::cend(arrayOfGuidsTwo)));

        // vector
        std::vector<GUID> vectorOfGuidsOne{ test::Guids[0], test::Guids[1] };
        std::vector<GUID> vectorOfGuidsTwo{ test::Guids[0], test::Guids[1], test::Guids[2] };
        REQUIRE(std::equal(std::cbegin(vectorOfGuidsOne), std::cend(vectorOfGuidsOne), std::cbegin(vectorOfGuidsOne)));
        REQUIRE(!std::equal(std::cbegin(vectorOfGuidsOne), std::cend(vectorOfGuidsOne), std::cbegin(vectorOfGuidsTwo), std::cend(vectorOfGuidsTwo)));
        vectorOfGuidsOne.push_back(test::Guids[2]);
        REQUIRE(std::equal(std::cbegin(vectorOfGuidsOne), std::cend(vectorOfGuidsOne), std::cbegin(vectorOfGuidsTwo)));
        vectorOfGuidsOne.pop_back();
        REQUIRE(!std::equal(std::cbegin(vectorOfGuidsOne), std::cend(vectorOfGuidsOne), std::cbegin(vectorOfGuidsTwo), std::cend(vectorOfGuidsTwo)));
    }

    SECTION("GUID can be used in associative containers")
    {
        {   // set
            std::array<std::set<GUID>, 2> setsOfGuids = {
                std::set<GUID>{ test::Guids[0], test::Guids[1] },
                std::set<GUID>{ test::Guids[1], test::Guids[0] },
            };
            REQUIRE(setsOfGuids[0] == setsOfGuids[1]);

            // verify correct ordering
            REQUIRE(std::equal(std::cbegin(setsOfGuids[0]), std::cend(setsOfGuids[0]), std::cbegin(arrayOfOrderedGuids)));
            REQUIRE(std::equal(std::cbegin(setsOfGuids[1]), std::cend(setsOfGuids[1]), std::cbegin(arrayOfOrderedGuids)));

            // add element to make sets unequal
            const auto [iterator, inserted] = setsOfGuids[1].insert(test::Guids[2]);
            REQUIRE(inserted == true);
            REQUIRE(*iterator == test::Guids[2]);
            REQUIRE(setsOfGuids[0] != setsOfGuids[1]);

            // remove added element
            const auto guidNodeOne = setsOfGuids[1].extract(test::Guids[2]);
            REQUIRE(!guidNodeOne.empty());
            REQUIRE(guidNodeOne.value() == test::Guids[2]);
            REQUIRE(setsOfGuids[0] == setsOfGuids[1]);

            // remove non-existent element
            const auto guidNodeTwo = setsOfGuids[0].extract(test::Guids[2]);
            REQUIRE(guidNodeTwo.empty());

            // swap and verify equality
            setsOfGuids[0].swap(setsOfGuids[1]);
            REQUIRE(setsOfGuids[0] == setsOfGuids[1]);
        }

        {   // map
            std::array<std::map<int, GUID>, 2> mapsOfGuids = {
                std::map<int, GUID>{ { 1, test::Guids[0] }, { 2, test::Guids[1] } },
                std::map<int, GUID>{ { 1, test::Guids[0] }, { 2, test::Guids[1] } },
            };
            
            REQUIRE(mapsOfGuids[0] == mapsOfGuids[1]);

            // add element to make maps unequal
            const auto [iterator, inserted] = mapsOfGuids[1].insert({ 3, test::Guids[2] });
            REQUIRE(inserted == true);
            const auto [key, value] = *iterator;
            REQUIRE(key == 3);
            REQUIRE(value == test::Guids[2]);
            REQUIRE(mapsOfGuids[0] != mapsOfGuids[1]);

            // remove added element
            const auto guidNodeOne = mapsOfGuids[1].extract(3);
            REQUIRE(!guidNodeOne.empty());
            REQUIRE(guidNodeOne.key() == 3);
            REQUIRE(guidNodeOne.mapped() == test::Guids[2]);
            REQUIRE(mapsOfGuids[0] == mapsOfGuids[1]);
        }

        {   // multiset
        }
    }

    SECTION("GUID can be used in unordered associative containers")
    {
        {   // unordered_set
            std::array<std::unordered_set<GUID>, 2> usetsOfGuids = {
                std::unordered_set<GUID>{ test::Guids[0], test::Guids[1] },
                std::unordered_set<GUID>{ test::Guids[0], test::Guids[1] },
            };
            REQUIRE(usetsOfGuids[0] == usetsOfGuids[1]);

            // add element to make sets unequal
            const auto [iterator, inserted] = usetsOfGuids[1].insert(test::Guids[2]);
            REQUIRE(inserted == true);
            REQUIRE(*iterator == test::Guids[2]);
            REQUIRE(usetsOfGuids[0] != usetsOfGuids[1]);

            // remove added element
            const auto guidNodeOne = usetsOfGuids[1].extract(test::Guids[2]);
            REQUIRE(!guidNodeOne.empty());
            REQUIRE(guidNodeOne.value() == test::Guids[2]);
            REQUIRE(usetsOfGuids[0] == usetsOfGuids[1]);

            // remove non-existent element
            const auto guidNodeTwo = usetsOfGuids[0].extract(test::Guids[2]);
            REQUIRE(guidNodeTwo.empty());

            // swap and verify equality
            usetsOfGuids[0].swap(usetsOfGuids[1]);
            REQUIRE(usetsOfGuids[0] == usetsOfGuids[1]);
        }

        {   // unordered_map
            std::array<std::unordered_map<int, GUID>, 2> umapsOfGuids = {
                std::unordered_map<int, GUID>{ { 1, test::Guids[0] }, { 2, test::Guids[1] } },
                std::unordered_map<int, GUID>{ { 1, test::Guids[0] }, { 2, test::Guids[1] } },
            };
            REQUIRE(umapsOfGuids[0] == umapsOfGuids[1]);

            // add element to make maps unequal
            const auto [iterator, inserted] = umapsOfGuids[1].insert({ 3, test::Guids[2] });
            REQUIRE(inserted == true);
            const auto [key, value] = *iterator;
            REQUIRE(key == 3);
            REQUIRE(value == test::Guids[2]);
            REQUIRE(umapsOfGuids[0] != umapsOfGuids[1]);

            // remove added element
            const auto guidNodeOne = umapsOfGuids[1].extract(3);
            REQUIRE(!guidNodeOne.empty());
            REQUIRE(guidNodeOne.key() == 3);
            REQUIRE(guidNodeOne.mapped() == test::Guids[2]);
            REQUIRE(umapsOfGuids[0] == umapsOfGuids[1]);
        }

        {   // unordered_multiset
        }
    }

    SECTION("GUID can be used in container adaptors")
    {
        // priority_queue
        std::array<std::priority_queue<GUID>, 2> priorityQueuesOfGuids = {
            std::priority_queue<GUID>(std::cbegin(arrayOfOrderedGuids), std::cend(arrayOfOrderedGuids)),
            std::priority_queue<GUID>{ isGuidLess, { test::Guids[2], test::Guids[1] } },
        };

        REQUIRE(priorityQueuesOfGuids[0].size() == 3);
        REQUIRE(priorityQueuesOfGuids[0].top() == test::Guids[2]);
        priorityQueuesOfGuids[0].pop();
        REQUIRE(priorityQueuesOfGuids[0].top() == test::Guids[1]);
        priorityQueuesOfGuids[0].pop();
        priorityQueuesOfGuids[0].pop();
        REQUIRE(priorityQueuesOfGuids[0].empty());
        priorityQueuesOfGuids[0].push(test::Guids[2]);
        priorityQueuesOfGuids[0].push(test::Guids[0]);
        priorityQueuesOfGuids[0].push(test::Guids[1]);
        REQUIRE(priorityQueuesOfGuids[0].top() == test::Guids[2]);

        REQUIRE(priorityQueuesOfGuids[1].size() == 2);
        REQUIRE(priorityQueuesOfGuids[1].top() == test::Guids[2]);
        priorityQueuesOfGuids[1].push(test::Guids[0]);
        REQUIRE(priorityQueuesOfGuids[1].size() == 3);
        REQUIRE(priorityQueuesOfGuids[1].top() == test::Guids[2]);
        priorityQueuesOfGuids[1].pop();
        priorityQueuesOfGuids[1].pop();
        REQUIRE(priorityQueuesOfGuids[1].top() == test::Guids[0]);
        priorityQueuesOfGuids[1].push(test::Guids[1]);
        REQUIRE(priorityQueuesOfGuids[1].top() == test::Guids[1]);
    }

    SECTION("GUID can be used with algorithms")
    {
        // find and find_if
        for (const auto& guidToFind : test::Guids) {
            const auto findResult = std::find(std::cbegin(test::Guids), std::cend(test::Guids), guidToFind);
            REQUIRE(findResult != std::cend(test::Guids));
            REQUIRE(*findResult == guidToFind);

            const auto findIfResult = std::find_if(std::cbegin(test::Guids), std::cend(test::Guids), [&](const auto& guidToCheck) {
                return (guidToCheck == guidToFind);
            });
            REQUIRE(findIfResult != std::cend(test::Guids));
            REQUIRE(*findIfResult == guidToFind);
        }

        // remove and erase
        std::vector<GUID> vectorOfGuids{ std::cbegin(test::Guids), std::cend(test::Guids) };
        for (const auto& guidToRemove : test::Guids) {
            const auto size = vectorOfGuids.size();
            vectorOfGuids.erase(std::remove_if(std::begin(vectorOfGuids), std::end(vectorOfGuids), [&](const auto& guidToCheck) {
                return (guidToCheck == guidToRemove);
            }), std::end(vectorOfGuids));
            REQUIRE(vectorOfGuids.size() == size - 1);
        }
        REQUIRE(vectorOfGuids.empty());
    }
}
