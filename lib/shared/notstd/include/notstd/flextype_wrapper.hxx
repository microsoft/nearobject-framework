
#ifndef FLEXTYPE_WRAPPER_HXX
#define FLEXTYPE_WRAPPER_HXX

#include <cassert>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory>
#include <span>
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
 * struct element {};
 * struct element_container {
 *     int index;
 *     std::size_t num_elements;
 *     element elements[];
 * };
 *
 * This type allocates a buffer to contain the wrapped type plus the specified
 * number of elements in its trailing flexible array member. The buffer size
 * calculation avoids the use of macros (eg. offsetof) for a cleaner
 * implementation at the cost of small over-allocation in cases where
 * padding is inserted into the containing structure.
 *
 * Important: This type *must not* be used with types that have nested flex-array
 * members since it uses 'sizeof(FlexElementT)' to determine the total required
 * size. This limitation will be removed once this class is updated to be created
 * with a total size instead of just the number of wrapped elements.
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
     *  1. The size of the wrapped type on its own.
     *  2. The size of the specified number of flex-array elements that follow it.
     *  3. The adjusted byte count for the flex-array declaration type.
     *
     * In case the flex-array uses a pre-C99 'anysize' declaration, the wrapped
     * type will include storage for a single flex-array element. In this case,
     * that storage space is subtracted from the total required. Otherwise, the
     * C99 conforming flex-array declaration does not require adjustment and no
     * storage is subtracted.
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
     * @brief Create a flextype_wrapper instance with enough room to store a
     * specified number of flex-array elements (element_type). This is purely a
     * conveneience factory function and must only be used for wrapped types
     * whose element_type itself does not contain a flex-array member (ie. no
     * nested flex-array types). This is because this function uses
     * 'sizeof(element_type)' in the total size calculation, and the sizeof
     * expression for types containing a flex-array member are incorrect by
     * definition.
     *
     * For nested flex-array types, the caller must recursively compute the
     * size of the wrapped type pass this to the constructor accepting the total
     * size instead.
     *
     * @param num_flex_elements Desired number of flex-array elements to store.
     * @return flextype_wrapper
     */
    static flextype_wrapper
    from_num_elements(std::size_t num_flex_elements)
    {
        return flextype_wrapper{ required_buffer_size(num_flex_elements) };
    }

    /**
     * @brief Given an input vector, return a pointer aligned to value_type.
     *
     * @param buffer The vector containing the buffer to align.
     * @param num_bytes_to_align The number of bytes in the buffer to align.
     * @return value_type*
     */
    static inline value_type*
    aligned_buffer(std::vector<uint8_t>& buffer, std::size_t num_bytes_to_align)
    {
        std::size_t space = std::size(buffer);
        void* aligned_pointer = std::data(buffer);
        return reinterpret_cast<value_type*>(std::align(alignof(value_type), num_bytes_to_align, aligned_pointer, space));
    }

    /**
     * @brief Construct a new flextype wrapper object with enough room for the
     * specified number of flex-array elements.
     *
     * @param num_flex_elements The number of flex-array elements to acommoodate.
     */
    explicit flextype_wrapper(std::size_t total_size) :
        m_buffer(alignof(value_type) + total_size),
        m_value(*aligned_buffer(m_buffer, total_size)),
        m_data(reinterpret_cast<uint8_t*>(&m_value), total_size)
    {
        assert(reinterpret_cast<uintptr_t>(&m_value) % alignof(decltype(m_value)) == 0);
    }

    /**
     * @brief Construct a new flextype wrapper object from a pre-existing value.
     *
     * @param total_size The total size of the pre-existing value.
     * @param value The pre-existing value to copy.
     */
    flextype_wrapper(std::size_t total_size, const value_type& value) :
        flextype_wrapper(total_size)
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
     * @brief The total size of the value. Note, the buffer may be larger than
     * this value since it has enough space to guarantee correct alignment.
     *
     * @return std::size_t
     */
    std::size_t
    size() const noexcept
    {
        return std::size(m_data);
    }

    /**
     * @brief The buffer containing the wrapped type.
     *
     * @return std::span<uint8_t>
     */
    std::span<uint8_t>
    data() noexcept
    {
        return m_data;
    }

private:
    std::vector<uint8_t> m_buffer;
    value_type& m_value;
    std::span<uint8_t> m_data;
};
} // namespace notstd

#endif // FLEXTYPE_WRAPPER_HXX
