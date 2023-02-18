
#ifndef FLEXTYPE_WRAPPER_HXX
#define FLEXTYPE_WRAPPER_HXX

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <type_traits>
#include <vector>

#include <notstd/utility.hxx>

namespace notstd
{
/**
 * @brief Type of flex-array.
 *
 * zero_length: This is the official C99 form: 'int flexArray[]'
 * anysize: This is the pre-C99 form: 'int flexArray[1]'
 */
enum class flex_array_type : std::size_t {
    zero_length = 0,
    anysize = 1,
};

/**
 * @brief Wrapper for "flexible array" types. That is, those that have a
 * flexible array as their last member. Eg.
 *
 * struct Element {};
 * struct ElementContainer {
 *     int index;
 *     std::size_t numElements;
 *     Element elements[];
 * };
 *
 * This type allocates a buffer to contain the wrapped type plus the specified
 * number of elements in its trailing flexible array member. The buffer size
 * calculation avoids the use of macros (eg. offsetof) for a cleaner
 * implementation at the cost of small over-allocation in cases where
 * padding is inserted into the containing structure.
 *
 * @tparam ValueT The type being wrapped.
 * @tparam FlexElementT The array type.
 * @tparam FlexElementAdjuster The adjustment factor based on the flex array type.
 */
template <
    typename ValueT,
    typename FlexElementT,
    flex_array_type FlexElementAdjuster = flex_array_type::anysize>
    requires std::is_standard_layout_v<ValueT>
struct flextype_wrapper
{
    using value_type = ValueT;
    using element_type = FlexElementT;
    static constexpr flex_array_type array_adjuster = FlexElementAdjuster;

    virtual ~flextype_wrapper() = default;

    /**
     * @brief The required buffer size.
     *
     * This calculates the size of the buffer required to store the wrapped type
     * (value_type) plus a specified number of flex-array elements
     * (element_type). The calculation does not account for padding in the
     * wrapped type and so will provide an over-estimate in these scenarios.
     * 
     * The sum consists of:
     *  1. The complete size of the wrapped type on its own.
     *  2. The number of flex-array elements that will follow it.
     *  3. The adjusted byte count for the flex-array declaration.
     *
     * In case the flex-array is a pre-C99 'anysize' type, the wrapped structure
     * will include storage for a single flex-array element. In this case, that
     * storage space is subtracted from the total required.
     *
     * The preferred approach to calculate the size is to use compile-time
     * reflection referencing the structure field offset of the flex-array type
     * indexed to the number of elements. Eg.
     *
     *  size = offsetof(value_type, flex_array_field_name[num_elements])
     *
     * However, specifying 'flex_array_field_name' cannot (AFAIK) be supplied as
     * a template argument. Thus, the calculation method decribed above is used
     * instead as a compromise.
     *
     * @param num_elements The desired number of elements to store following the
     * wrapped type.
     * @return constexpr auto
     */
    static inline constexpr auto
    required_buffer_size(std::size_t num_elements)
    {
        return sizeof(value_type) + (sizeof(element_type) * (num_elements - notstd::to_underlying(array_adjuster)));
    }

    /**
     * @brief Construct a new flextype wrapper object with enough room for the
     * specified number of flex-array elements.
     *
     * @param num_flex_elements The number of flex-array elements to acommoodate.
     */
    flextype_wrapper(std::size_t num_flex_elements) :
        m_buffer(required_buffer_size(num_flex_elements)),
        m_value(*reinterpret_cast<ValueT*>(std::data(m_buffer)))
    {}

    /**
     * @brief Construct a new flextype wrapper object from a pre-existing value.
     *
     * @param num_flex_elements The number of flexible array elements in the value.
     * @param value The pre-existing value to copy.
     *
     * TODO: the caller knows the exact size of the structure, so we should
     * instead just take in that value instead, avoiding any possible
     * overallocation due to padding. It probably makes sense to convert this to
     * a static function instead.
     */
    flextype_wrapper(std::size_t num_flex_elements, const value_type& value) :
        flextype_wrapper(num_flex_elements)
    {
        std::memcpy(&m_value, &value, size());
    }

    /**
     * @brief User-defined operator allowing the wrapper to be implicitly
     * converted to the wrapped type.
     *
     * @return value_type&
     */
    operator value_type&() noexcept
    {
        return m_value;
    }

    /**
     * @brief The total size of the buffer. Note that this may be larger than
     * the size required to contain all of the flex-array elements since the
     * size calculation does not calculate the length accurately to avoid
     * requiring the field name and use of a macro.
     *
     * @return std::size_t
     */
    std::size_t
    size() const noexcept
    {
        return std::size(m_buffer);
    }

    /**
     * @brief The buffer containing the wrapped type.
     *
     * @return std::vector<uint8_t>&
     */
    std::vector<uint8_t>&
    data() noexcept
    {
        return m_buffer;
    }

private:
    std::vector<uint8_t> m_buffer;
    value_type& m_value;
};
} // namespace notstd

#endif // FLEXTYPE_WRAPPER_HXX
