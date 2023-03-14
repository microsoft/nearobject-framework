
#ifndef NOT_STD_RANGE_HXX
#define NOT_STD_RANGE_HXX

namespace notstd
{
/**
 * @brief Helper class to enable use of iterators in range-based for loops.
 * 
 * This is mostly equivalent to wil::make_range(), but exists here such that it
 * can be used across different operating systems.
 * 
 * Eg. Make a range from a raw pointer.
 * 
 * void printNumbers(int *valuesArray, std::size_t valuesCount) {
 *      for (const auto& value : notstd::make_range(valuesArray, valuesCount)) {
 *          std::cout << "value: " << value << std::endl;
 *      }
 * }
 *
 * @tparam IteratorT The type of iterator to use. This will typically be deduced
 * and so there is generally no need to specify it explicitly.
 */
template <typename IteratorT>
class iterator_range
{
public:
    iterator_range(IteratorT begin, IteratorT end) :
        m_begin(begin),
        m_end(end)
    {}

    constexpr IteratorT
    begin() const noexcept
    {
        return m_begin;
    }

    constexpr IteratorT
    end() const noexcept
    {
        return m_end;
    }

private:
    IteratorT m_begin;
    IteratorT m_end;
};

/**
 * @brief Creates an iterator range from beginning and ending iterators.
 *
 * @tparam IteratorT 
 * @param begin An iterator to the starting item.
 * @param end An iterator to the ending item.
 * @return constexpr auto 
 */
template <typename IteratorT>
constexpr auto
make_range(IteratorT begin, IteratorT end) noexcept
{
    return iterator_range<IteratorT>(begin, end);
}

/**
 * @brief Creates an iterator range from beginning iterator and a count of the
 * number of items to include. The count must be valid.
 * 
 * @tparam IteratorT 
 * @param begin An iterator to the starting item.
 * @param count The number of items the range should comprise.
 * @return constexpr auto 
 */
template <typename IteratorT>
constexpr auto
make_range(IteratorT begin, std::size_t count) noexcept
{
    return iterator_range<IteratorT>(begin, begin + count);
}

} // namespace notstd

#endif // NOT_STD_RANGE_HXX
