
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

enum class flex_array_type : std::size_t {
    ZeroLength = 0,
    Anysize = 1,
};

template <
    typename ValueT,
    typename FlexElementT,
    flex_array_type FlexElementAdjuster = flex_array_type::Anysize
>
requires std::is_standard_layout_v<ValueT> && std::is_standard_layout_v<FlexElementT>
struct flextype_wrapper
{
    using value_type = ValueT;
    using element_type = FlexElementT;
    static constexpr flex_array_type array_adjuster = FlexElementAdjuster;

    virtual ~flextype_wrapper() = default;

    flextype_wrapper(std::size_t numFlexElements) :
        m_buffer(sizeof(ValueT) + (sizeof(FlexElementT) * (numFlexElements - notstd::to_underlying(FlexElementAdjuster)))),
        m_value(*reinterpret_cast<ValueT*>(std::data(m_buffer)))
    {}

    flextype_wrapper(std::size_t numFlexElements, const ValueT& value) :
        flextype_wrapper(numFlexElements)
    {
        std::memcpy(&m_value, &value, Size());
    }

    operator ValueT&() noexcept
    {
        return m_value;
    }

    std::size_t
    Size() const noexcept
    {
        return std::size(m_buffer);
    }

    std::vector<uint8_t>&
    Buffer() noexcept
    {
        return m_buffer;
    }

private:
    std::vector<uint8_t> m_buffer;
    ValueT& m_value;
};
} // namespace notstd

#endif // FLEXTYPE_WRAPPER_HXX
