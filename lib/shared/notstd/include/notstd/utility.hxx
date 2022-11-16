
#ifndef NOTSTD_UTILITY
#define NOTSTD_UTILITY

#include <type_traits>

namespace notstd
{
/**
 * @brief Converts a enumeration value to its underlying type.
 * 
 * The function prototype matches the c++23 function so can be used in its
 * place.
 * 
 * @tparam Enum The enumeration class type to convert.
 * @param e The enumeration class value.
 * @return constexpr std::underlying_type_t<Enum> 
 */
template <typename Enum>
constexpr std::underlying_type_t<Enum>
to_underlying(Enum e) noexcept
{
    return static_cast<std::underlying_type_t<Enum>>(e);
}
} // namespace notstd

#endif // NOTSTD_UTILITY
