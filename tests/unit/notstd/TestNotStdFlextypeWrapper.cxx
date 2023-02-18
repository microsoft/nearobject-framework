
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <type_traits>
#include <utility>

#include <catch2/catch_test_macros.hpp>

#include <notstd/flextype_wrapper.hxx>
#include <notstd/utility.hxx>

namespace notstd::test
{
template <
    typename ElementT,
    flex_array_type ElementTArrayIndex = flex_array_type::Anysize
>
struct test_flex_type
{
    using flex_element_t = ElementT;
    static constexpr flex_array_type flex_element_type_array_index = ElementTArrayIndex;

    uint32_t value;
    std::size_t num_elements;
    flex_element_t elements[notstd::to_underlying(flex_element_type_array_index)];

    // template <std::size_t NumElements>
    // static inline constexpr std::size_t total_size()
    // {
    //     return offsetof(test_flex_type, elements[NumElements]);
    // }

    template <std::size_t NumElements>
    struct total_size : public std::integral_constant<std::size_t, NumElements>
    {};
};

template <
    typename ElementT,
    flex_array_type FlexElementAdjuster = flex_array_type::Anysize
>
struct test_flex_wrapper : 
    public notstd::flextype_wrapper<test_flex_type<ElementT, FlexElementAdjuster>, ElementT, FlexElementAdjuster>
{
    using value_type = test_flex_type<ElementT, FlexElementAdjuster>;
    using flex_wrapper_type = flextype_wrapper<value_type, ElementT, FlexElementAdjuster>;

    test_flex_wrapper(std::size_t numElements) :
        flex_wrapper_type(numElements)
    {}
};

using test_flex_type_element_byte = uint8_t;

struct test_flex_type_element_compound
{
    uint32_t Data1;
    uint8_t Data2;
};
} // namespace notstd::test

TEST_CASE("flextype_wrapper can be used as value container", "[basic]")
{
    using namespace notstd;
    using namespace notstd::test;

    static constexpr std::size_t NumElements = 20;

    SECTION("value type is correctly reflected with single byte flex-element")
    {
        test_flex_wrapper<test_flex_type_element_byte> wrapper{ NumElements };
    }

    SECTION("value type is correctly reflected with compound flex-element")
    {
        using flex_wrapper_type = test_flex_wrapper<test_flex_type_element_compound>;

        constexpr static auto SizeTotal = flex_wrapper_type::value_type::total_size<NumElements>();

        flex_wrapper_type wrapper{NumElements};

        // Ensure the total size matches.
        REQUIRE(wrapper.Size() == SizeTotal);

        // Populate the value.
        flex_wrapper_type::value_type& value = wrapper;
        value.num_elements = NumElements;
        for (uint8_t i = 0U; i < NumElements; i++) {
            value.elements[i] = { i, i };
        }

        // Verify the value in the buffer reflects the populated value.
        auto& buffer = wrapper.Buffer();
        flex_wrapper_type::value_type& valueFromBuffer = *reinterpret_cast<flex_wrapper_type::value_type*>(std::data(buffer));
        REQUIRE(std::memcmp(&valueFromBuffer, &value, SizeTotal) == 0);
    }
}

TEST_CASE("flextype_wrapper can be used with an existing value", "[basic]")
{
    using namespace notstd::test;

    SECTION("wrapped value matches original value")
    {
    }
}
