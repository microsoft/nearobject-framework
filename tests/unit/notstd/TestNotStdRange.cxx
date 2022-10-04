
#include <array>
#include <type_traits>
#include <vector>

#include <catch2/catch.hpp>
#include <notstd/range.hxx>

// NOLINTBEGIN(*-avoid-c-arrays, cppcoreguidelines-pro-bounds-pointer-arithmetic)

/**
 * @brief Helper to validate a created range.
 * 
 * @tparam IteratorT 
 * @param range The range to validate.
 * @param begin The expected begin iterator value.
 * @param end The expected end iterator value.
 */
template <typename IteratorT>
void
ValidateRange(const notstd::iterator_range<IteratorT>& range, const IteratorT& begin, const IteratorT& end)
{
    REQUIRE(range.begin() == begin);
    REQUIRE(range.end() == end);
}

/**
 * @brief Count-based helper as above.
 * 
 * @tparam IteratorT 
 * @param range 
 * @param begin 
 * @param count 
 */
template <typename IteratorT>
void
ValidateRange(const notstd::iterator_range<IteratorT>& range, const IteratorT& begin, std::size_t count)
{
    return ValidateRange<IteratorT>(range, begin, begin + count);
}

/**
 * @brief Helper macro for using the same integer value sequence across
 * multiple tests. This is needed because std::to_array<> isn't available until
 * C++ 20.
 */
#define VALUE_SEQUENCE 1,2,3,4,5,6,7,8,9,10,11,12

namespace notstd
{
namespace test
{
    // Raw array
    constexpr int ValuesRawArray[] = { VALUE_SEQUENCE };
    constexpr auto *ValuesRawArrayPtr = &ValuesRawArray[0];
    constexpr auto ValuesRawCount = std::extent<decltype(ValuesRawArray)>::value;
    constexpr auto ValuesRawPBegin = &ValuesRawArray[0];
    constexpr auto ValuesRawBegin = std::begin(ValuesRawArray);
    constexpr auto ValuesRawCBegin = std::cbegin(ValuesRawArray);
    constexpr auto ValuesRawEnd = std::end(ValuesRawArray);
    constexpr auto ValuesRawCEnd = std::cend(ValuesRawArray);

    // std::array
    constexpr std::array ValuesStdArray = { VALUE_SEQUENCE };
    constexpr auto ValuesStdArrayCount = std::size(ValuesStdArray);
    constexpr auto ValuesStdArrayBegin = std::begin(ValuesStdArray);
    constexpr auto ValuesStdArrayCBegin = std::cbegin(ValuesStdArray);
    constexpr auto ValuesStdArrayEnd = std::end(ValuesStdArray);
    constexpr auto ValuesStdArrayCEnd = std::cend(ValuesStdArray);

} // namespace test
} // namespace notstd

// This test specifically tests iterators which use arithmetic, so disable lint for that.
TEST_CASE("range can be created from range bounds", "[notstd][range][utility]")
{
    using namespace notstd::test;

    SECTION("range from bounds using c'tor")
    {
        const notstd::iterator_range rangeByAddress{ ValuesRawPBegin, ValuesRawEnd };
        const notstd::iterator_range rangeByFunction{ ValuesRawBegin, ValuesRawEnd };
        const notstd::iterator_range rangeByFunctionConst{ ValuesRawCBegin, ValuesRawCEnd };
        const notstd::iterator_range rangeByPointer{ ValuesRawArrayPtr, ValuesRawArrayPtr + ValuesRawCount };

        ValidateRange(rangeByAddress, ValuesRawPBegin, ValuesRawEnd);
        ValidateRange(rangeByFunction, ValuesRawBegin, ValuesRawEnd);
        ValidateRange(rangeByFunctionConst, ValuesRawCBegin, ValuesRawCEnd);
        ValidateRange(rangeByFunctionConst, ValuesRawArrayPtr, ValuesRawArrayPtr + ValuesRawCount);
    }

    SECTION("range from pointer using make_range")
    {
        const auto rangeByAddress = notstd::make_range(ValuesRawPBegin, ValuesRawEnd);
        const auto rangeByFunction = notstd::make_range(ValuesRawBegin, ValuesRawEnd);
        const auto rangeByFunctionConst = notstd::make_range(ValuesRawCBegin, ValuesRawCEnd);
        const auto rangeByPointer = notstd::make_range(ValuesRawArrayPtr, ValuesRawArrayPtr + ValuesRawCount);
        const auto rangeByPointerCount = notstd::make_range(ValuesRawArrayPtr, ValuesRawCount);

        ValidateRange(rangeByAddress, ValuesRawPBegin, ValuesRawEnd);
        ValidateRange(rangeByFunction, ValuesRawBegin, ValuesRawEnd);
        ValidateRange(rangeByFunctionConst, ValuesRawCBegin, ValuesRawCEnd);
        ValidateRange(rangeByPointer, ValuesRawArrayPtr, ValuesRawArrayPtr + ValuesRawCount);
        ValidateRange(rangeByPointerCount, ValuesRawArrayPtr, ValuesRawArrayPtr + ValuesRawCount);
    }
}

TEST_CASE("range can be created from range start and count", "[notstd][range][utility]")
{
    using namespace notstd::test;

    SECTION("range from start and count using make_range")
    {
        const auto rangeByFunction = notstd::make_range(ValuesStdArrayBegin, ValuesStdArrayCount);
        const auto rangeByFunctionConst = notstd::make_range(ValuesStdArrayCBegin, ValuesStdArrayCount);

        ValidateRange(rangeByFunction, ValuesStdArrayBegin, ValuesStdArrayCount);
        ValidateRange(rangeByFunctionConst, ValuesStdArrayCBegin, ValuesStdArrayCount);
    }
}

TEST_CASE("range can be used in range-based for loops")
{
    using namespace notstd::test;

    SECTION("range created by c'tor can be used range-base for loop")
    {
        REQUIRE_NOTHROW([&]{
            for (const auto& value : notstd::iterator_range(ValuesStdArrayBegin, ValuesStdArrayEnd)) {
                ;
            }
        }());
        REQUIRE_NOTHROW([&]{
            for (const auto& value : notstd::iterator_range(ValuesStdArrayCBegin, ValuesStdArrayCEnd)) {
                ;
            }
        }());
    }

    SECTION("range created by make_range can be used in range-based for loop")
    {
        REQUIRE_NOTHROW([&]{
            for (const auto& value : notstd::make_range(ValuesStdArrayBegin, ValuesStdArrayCount)) {
                ;
            }
        }());
        REQUIRE_NOTHROW([&]{
            for (const auto& value : notstd::make_range(ValuesStdArrayCBegin, ValuesStdArrayCount)) {
                ;
            }
        }()); 
    }
}

// NOLINTEND(*-avoid-c-arrays, cppcoreguidelines-pro-bounds-pointer-arithmetic)
