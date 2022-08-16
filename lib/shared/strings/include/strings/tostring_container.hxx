
#ifndef TO_STRING_CONTAINER_HXX
#define TO_STRING_CONTAINER_HXX

#include <iterator>
#include <sstream>
#include <string>

/**
 * @brief Helper to stringify a container of items, using a comman for a delimeter.
 * 
 * @tparam ContainerT 
 * @tparam ContainerT::value_type 
 * @param items The container of items to stringify.
 * @param itemName The name of the parent item hosting the container.
 * @param emptyName The name to output if the container is empty.
 * @return auto 
 */
template <
    typename ContainerT,
    typename ItemT = typename ContainerT::value_type
>
auto ToString(const ContainerT& items, const char* itemName, const char* emptyName = "(empty)")
{
    std::ostringstream output;
    if (items.empty()) {
        output << itemName << ": " << emptyName;
    }
    else {
        output << itemName << ": " << *std::cbegin(items);
        for (auto it = std::next(std::cbegin(items)); it != std::cend(items); it = std::next(it)) {
            output << ", " << itemName << ": " << *it;
        }
    }

    return output.str();
}

#endif // TO_STRING_CONTAINER_HXX
