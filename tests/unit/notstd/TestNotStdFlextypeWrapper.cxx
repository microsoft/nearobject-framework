
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <tuple>

#include <catch2/catch_test_macros.hpp>

#include <notstd/flextype_wrapper.hxx>
#include <notstd/utility.hxx>

namespace notstd::test
{
/**
 * @brief Helper type which defines a type to be used with the flextype_wrapper.
 * The number of flex-array elements must be known at compile time, which is
 * fine for testing purposes.
 *
 * @tparam ElementT The flex-array element type.
 * @tparam ElementTArrayIndex The type of flex-array (index) to use.
 */
template <
    typename ElementT,
    flex_array_type ElementTArrayIndex = flex_array_type::anysize>
struct test_flex_type
{
    using flex_element_t = ElementT;
    static constexpr flex_array_type flex_element_type_array_index = ElementTArrayIndex;

    // Note: the types and widths here are intentional to ensure that padding is
    // added between the 'value' member (32-bit alignment) and the
    // 'num_elements' member (64-bit alignment). This ensures the structure's
    // size will always be greater than the sum of the size of its members.
    uint32_t value;
    std::size_t num_elements;
    flex_element_t elements[notstd::to_underlying(flex_element_type_array_index)];

    /**
     * @brief 
     * 
     * @param other 
     * @return true 
     * @return false 
     */
    bool
    operator==(const test_flex_type& other) const noexcept
    {
        return std::tie(this->value, this->num_elements) == std::tie(other.value, other.num_elements) && 
            std::memcmp(&this->elements[0], &other.elements[0], other.num_elements * sizeof elements[0]) == 0;
    }

    /**
     * @brief Helper template function to calculate the total size of the
     * wrapped structure including a specified number of flex-array elements.
     *
     * This returns the exact size required since it uses the offsetof macro,
     * which uses compile-time reflection.
     *
     * @tparam num_elements The desired number of elements in the flex-array.
     */
    template <std::size_t num_elements>
    struct total_size :
        public std::integral_constant<std::size_t, offsetof(test_flex_type, elements[num_elements])>
    {};
};

/**
 * @brief Helper type which composes a flextype_wrapper using the test_flex_type helper.
 *
 * @tparam ElementT The flex-array element type.
 * @tparam ElementTArrayIndex The type of flex-array (index) to use.
 */
template <
    typename ElementT,
    flex_array_type FlexElementAdjuster = flex_array_type::anysize>
struct test_flex_wrapper :
    public notstd::flextype_wrapper<test_flex_type<ElementT, FlexElementAdjuster>, ElementT, FlexElementAdjuster>
{
    using value_type = test_flex_type<ElementT, FlexElementAdjuster>;
    using flex_wrapper_type = flextype_wrapper<value_type, ElementT, FlexElementAdjuster>;

    test_flex_wrapper(std::size_t numElements) :
        flex_wrapper_type(numElements)
    {}
};

/**
 * @brief Simple single-byte type for use in a flex-array.
 */
struct test_flex_type_element_byte
{
    uint8_t data{ 0xFFU };
};

/**
 * @brief Complex multi-byte type for use in a flex-array.
 */
struct test_flex_type_element_compound
{
    uint32_t data1{ 0xDEADBEEFU };
    uint8_t data2{ 0xADU };
};
} // namespace notstd::test

TEST_CASE("flextype_wrapper can be used as a value container with element-based creational pattern", "[basic]")
{
    using namespace notstd;
    using namespace notstd::test;

    static constexpr std::size_t num_elements = 5;

    // TODO: the test code for the below 2 sections is nearly identical except
    // for the population bits, so can likely be consolidated into a template
    // test validation function.

    SECTION("value type is correctly reflected with single byte flex-element")
    {
        using flex_wrapper_type = test_flex_wrapper<test_flex_type_element_byte>;

        auto wrapper = flex_wrapper_type::from_num_elements(num_elements);

        // Ensure there's enough room to store the complete type including flex array elements.
        REQUIRE(wrapper.size() >= flex_wrapper_type::value_type::total_size<num_elements>());

        flex_wrapper_type::value_type& value = wrapper;
        value.value = 0xAA55AA55U;
        value.num_elements = num_elements;
        for (uint8_t i = 0U; i < num_elements; i++) {
            value.elements[i] = { i };
        }

        auto buffer = wrapper.data();
        flex_wrapper_type::value_type& valueFromBuffer = *reinterpret_cast<flex_wrapper_type::value_type*>(std::data(buffer));
        REQUIRE(std::memcmp(&valueFromBuffer, &value, wrapper.size()) == 0);
    }

    SECTION("value type is correctly reflected with compound flex-element")
    {
        using flex_wrapper_type = test_flex_wrapper<test_flex_type_element_compound>;

        auto wrapper = flex_wrapper_type::from_num_elements(num_elements);

        // Ensure there's enough room to store the complete type including flex array elements.
        REQUIRE(wrapper.size() >= flex_wrapper_type::value_type::total_size<num_elements>());

        // Populate the value.
        flex_wrapper_type::value_type& value = wrapper;
        value.value = 0xFEEDF00DU;
        value.num_elements = num_elements;
        for (uint8_t i = 0U; i < num_elements; i++) {
            value.elements[i] = { i, i };
        }

        // Verify the value in the buffer reflects the populated value.
        auto buffer = wrapper.data();
        flex_wrapper_type::value_type& valueFromBuffer = *reinterpret_cast<flex_wrapper_type::value_type*>(std::data(buffer));
        REQUIRE(std::memcmp(&valueFromBuffer, &value, wrapper.size()) == 0);
    }

    SECTION("value type is correctly reflected in copied instance")
    {
        using flex_wrapper_type = test_flex_wrapper<test_flex_type_element_compound>;

        auto wrapper = flex_wrapper_type::from_num_elements(num_elements);
        flex_wrapper_type::value_type& value = wrapper;
        value.value = 0xFEEDF00DU;
        value.num_elements = num_elements;
        for (uint8_t i = 0U; i < num_elements; i++) {
            value.elements[i] = { i, i };
        }

        auto wrapper_copy{ wrapper };
        flex_wrapper_type::value_type& value_copy = wrapper_copy;
        REQUIRE(value == value_copy); 
    }

    SECTION("vlaue type is correctly reflected in moved instance")
    {
        using flex_wrapper_type = test_flex_wrapper<test_flex_type_element_compound>;

        auto wrapper = flex_wrapper_type::from_num_elements(num_elements);
        flex_wrapper_type::value_type& value = wrapper;
        value.value = 0xFEEDF00DU;
        value.num_elements = num_elements;
        for (uint8_t i = 0U; i < num_elements; i++) {
            value.elements[i] = { i, i };
        }

        auto wrapper_copy{ std::move(wrapper) };
        flex_wrapper_type::value_type& value_copy = wrapper_copy;
        REQUIRE(value == value_copy); 
    }
}

TEST_CASE("flextype_wrapper can be used as value container with size-based creational pattern", "[basic]")
{
    using namespace notstd::test;

    // TODO
}

TEST_CASE("flextype_wrapper can be used with an existing value", "[basic]")
{
    using namespace notstd::test;

    SECTION("wrapped value matches original value")
    {
        // TODO
    }
}
