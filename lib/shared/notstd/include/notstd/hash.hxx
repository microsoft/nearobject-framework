
#ifndef NOTSTD_HASH_HXX
#define NOTSTD_HASH_HXX

#include <cstddef>
#include <iterator>

namespace notstd
{
/**
 * @brief Hashes a series of values.
 * 
 * Note that the parameter pack may contain any number of types, they do not
 * have to be uniform. For example, the below is a valid input:
 * 
 *  hash_combine(9, 0x0123456789abcdef, true, -42, std::numbers::pi, "hello");
 * 
 * The implementation was mostly taken from boost::hash_combine.
 * 
 * @tparam T The value type to hash. This type must have a full specialization of std::hash<T>.
 * @tparam ValuesRemaining A parameter pack of the values to hash.
 * @param seed The initial value of the hash.
 * @param value The initial value to hash.
 * @param values The remaining values to hash.
 */
template <typename T, typename... ValuesRemaining>
inline void 
hash_combine(std::size_t& seed, const T& value, ValuesRemaining... values)
{
    seed ^= std::hash<T>{}(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    (hash_combine(seed, values), ...);
}

/**
 * @brief Hashes a range of values, specified by start and end iterators.
 * 
 * @tparam Iterator The type of iterator referring to the values.
 * @param seed The initial value of the hash.
 * @param first The iterator referring to the start of the range.
 * @param last The iterator referring to the end of the range.
 * 
 * The implementation was mostly taken from boost::hash_range.
 */
template <typename Iterator>
inline void
hash_range(std::size_t& seed, Iterator first, Iterator last)
{
    for (; first != last; ++first) {
        hash_combine<typename std::iterator_traits<Iterator>::value_type>(seed, *first);
    }
}

/**
 * @brief Hashes a range of values, specified by start and end iterators, with a
 * null seed value.
 * 
 * This should only be used when the seed is unimportant.
 * 
 * @tparam Iterator The type of iterator referring to the values.
 * @param first The iterator referring to the start of the range.
 * @param last The iterator referring to the end of the range.
 * @return std::size_t 
 */
template<typename Iterator> 
std::size_t 
hash_range(Iterator first, Iterator last)
{
    size_t seed = 0;
    hash_range(seed, first, last);
    return seed;
}

} // namespace notstd

#endif // NOTSTD_HASH_HXX
