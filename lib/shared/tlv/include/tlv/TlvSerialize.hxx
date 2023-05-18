
#ifndef TLV_SERIALIZE_HXX
#define TLV_SERIALIZE_HXX

#include <cstdint>
#include <span>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace encoding
{
/**
 * @brief Get the Bit Mask From Bit Index object
 *
 * @param bitIndex
 * @return std::size_t
 */
std::size_t
GetBitMaskFromBitIndex(std::size_t bitIndex);

/**
 * @brief Get the Bit Index From Bit Mask object.
 *
 * @param bitMask
 * @return std::size_t
 */
std::size_t
GetBitIndexFromBitMask(std::size_t bitMask);

/**
 * @brief Get the Bytes Big Endian From std::size_t
 *
 * @param value the bitmap to encode
 * @param desiredLength the desired number of bytes in the encoding, padding with zeros if necessary.
 *                      If the value is too large, this will only encode the lowest desiredLength bytes
 * @return std::vector<uint8_t>
 */
std::vector<uint8_t>
GetBytesBigEndianFromBitMap(std::size_t value, std::size_t desiredLength);

/**
 * @brief Parses a span of bytes as a std::size_t number encoded in big endian.
 *
 * @tparam IntegerT The type of output integer.
 * @param bytes The buffer to parse.
 * @return
 */
template <typename IntegerT = std::size_t>
// clang-format off
requires std::is_unsigned_v<IntegerT>
IntegerT
// clang-format on
ReadSizeTFromBytesBigEndian(std::span<const uint8_t> bytes)
{
    std::size_t rvalue = 0;

    if (bytes.size() >= sizeof rvalue) {
        return 0; // TODO throw error? this isn't really part of an interface so may not be necessary
    }

    for (std::size_t i = 0; i < bytes.size(); i++) {
        rvalue *= 0x100;
        rvalue += bytes[i];
    }
    return static_cast<IntegerT>(rvalue);
}

/**
 * @brief helper function that writes the bitset for a given valueSet according to bitIndexMap, using desiredLength bytes
 *
 * @tparam T
 * @param assignee the destination of the bitset
 * @param bitIndexMap
 * @param bytes
 */
template <class T>
std::vector<T>
AssignValuesFromBytes(const std::unordered_map<T, std::size_t>& bitIndexMap, std::span<const uint8_t> bytes)
{
    std::vector<T> assignee;
    auto bitmasks = ReadSizeTFromBytesBigEndian(bytes);
    for (const auto& [key, value] : bitIndexMap) {
        if (bitmasks & GetBitMaskFromBitIndex(value)) {
            assignee.push_back(key);
        }
    }
    return assignee;
}

/**
 * @brief helper function to encode a given valueSet into a bitset according to bitIndexMap, using desiredLength bytes
 *
 * @tparam T
 * @param valueSet
 * @param bitIndexMap
 * @param desiredLength
 * @return std::vector<uint8_t>
 */
template <class T>
std::vector<uint8_t>
EncodeValuesAsBytes(const std::vector<T>& valueSet, const std::unordered_map<T, std::size_t>& bitIndexMap, std::size_t desiredLength)
{
    std::size_t valueSetEncoded = 0;
    for (const auto& value : valueSet) {
        auto bitIndex = bitIndexMap.at(value);
        valueSetEncoded |= GetBitMaskFromBitIndex(bitIndex);
    }
    return encoding::GetBytesBigEndianFromBitMap(valueSetEncoded, desiredLength);
}

} // namespace encoding

#endif // TLV_SERIALIZE_HXX
