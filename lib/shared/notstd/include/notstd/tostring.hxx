
#ifndef TO_STRING_HXX
#define TO_STRING_HXX

#include <iostream>
#include <iterator>
#include <optional>
#include <sstream>
#include <string>
#include <type_traits>

namespace strings
{
namespace ostream_operators
{
namespace detail
{
struct empty_value_provider
{
    std::string
    operator()() const noexcept
    {
        return "(empty)";
    }
};
} // namespace detail

/**
 * @brief Helper implementing global operator << which passes the result of
 * ToString() to the output stream.
 * 
 * @tparam HasToStringT The type which implements a ToString() function returning std::string
 * @param stream 
 * @param ref 
 * @return std::ostream& 
 */
template <
    typename CharT,
    typename Traits,
    typename HasToStringT,
    typename = typename std::enable_if<std::is_member_function_pointer<decltype(&HasToStringT::ToString)>::value>::type>
std::ostream&
operator<<(std::basic_ostream<CharT, Traits>& stream, const HasToStringT& ref)
{
    stream << ref.ToString();
    return stream;
}

/**
 * @brief Helper implementing global operator <<, wrapping optional types whose
 * value type defines a ToString() function.
 * 
 * @tparam HasToStringT The type which implements a ToString() function returning std::string
 * @tparam EmptyValueProviderT The type providing the value to pass to the
 * output stream if the specified object does not contain a value. Defaults to
 * empty_value_provider
 * @param stream 
 * @param ref 
 * @return std::ostream& 
 */
template <
    typename CharT,
    typename Traits,
    typename HasToStringT,
    typename = typename std::enable_if<std::is_member_function_pointer<decltype(&HasToStringT::ToString)>::value>::type,
    typename EmptyValueProviderT = detail::empty_value_provider>
std::ostream&
operator<<(std::basic_ostream<CharT, Traits>& stream, const std::optional<HasToStringT>& ref)
{
    ref.has_value()
        ? stream << ref.value()
        : stream << EmptyValueProviderT{}();
    return stream;
}

namespace containers
{
/**
 * @brief Helper to stringify a container of items, using a comma for a delimeter.
 * 
 * @tparam ContainerT 
 * @tparam ContainerT::value_type 
 * @param items The container of items to stringify.
 * @param item_name The name of the parent item hosting the container.
 * @param empty_name The name to output if the container is empty.
 * @return auto 
 */
template <
    typename ContainerT,
    typename ItemT = typename ContainerT::value_type>
auto
to_string(const ContainerT& items, const char* item_name, const char* empty_name = "(empty)")
{
    std::ostringstream output;
    if (items.empty()) {
        output << item_name << ": " << empty_name;
    } else {
        output << item_name << ": " << *std::cbegin(items);
        for (auto it = std::next(std::cbegin(items)); it != std::cend(items); it = std::next(it)) {
            output << ", " << item_name << ": " << *it;
        }
    }

    return output.str();
}

} // namespace containers

} // namespace ostream_operators
} // namespace strings

#endif // TO_STRING_HXX
