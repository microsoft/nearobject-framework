
#ifndef TO_STRING_HXX
#define TO_STRING_HXX

#include <iostream>
#include <optional>
#include <string>
#include <type_traits>

struct EmptyValueProvider
{
    std::string operator()() const noexcept
    {
        return "(empty)";
    }
};

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
    typename HasToStringT,
    typename = typename std::enable_if<std::is_member_function_pointer<decltype(&HasToStringT::ToString)>::value>::type
>
std::ostream& operator<<(std::ostream& stream, const HasToStringT& ref)
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
 * EmptyValueProvider
 * @param stream 
 * @param ref 
 * @return std::ostream& 
 */
template <
    typename HasToStringT,
    typename = typename std::enable_if<std::is_member_function_pointer<decltype(&HasToStringT::ToString)>::value>::type,
    typename EmptyValueProviderT = EmptyValueProvider
>
std::ostream& operator<<(std::ostream& stream, const std::optional<HasToStringT>& ref)
{
    ref.has_value()
        ? stream << ref.value()
        : stream << EmptyValueProviderT{}();
    return stream;
}

#endif // TO_STRING_HXX
