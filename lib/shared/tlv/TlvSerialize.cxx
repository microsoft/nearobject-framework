
#include <iterator>
#include <stdexcept>

#include <tlv/TlvSerialize.hxx>

std::size_t
encoding::GetBitMaskFromBitIndex(std::size_t bitIndex)
{
    if (bitIndex >= sizeof(std::size_t) * CHAR_BIT) {
        return 0; // TODO throw error? this isn't really part of an interface so may not be necessary
    }
    return static_cast<std::size_t>(1UL) << bitIndex;
}

std::size_t
encoding::GetBitIndexFromBitMask(std::size_t bitMask)
{
    for (std::size_t index = 0; index < (sizeof bitMask) * CHAR_BIT; index++) {
        if (bitMask == GetBitMaskFromBitIndex(index)) {
            return index;
        }
    }
    throw std::runtime_error("bit index not found");
}

std::vector<uint8_t>
encoding::GetBytesBigEndianFromBitMap(std::size_t value, std::size_t desiredLength)
{
    if (desiredLength > sizeof value) {
        throw std::runtime_error("desired length exceeds std::size_t width, this is a bug!");
    }

    std::vector<uint8_t> bytes;
    bytes.reserve(desiredLength);

    for (std::size_t i = 0; i < desiredLength; i++) {
        const auto b = static_cast<uint8_t>(value & 0xFFU);
        bytes.push_back(b);
        value >>= 8U;
    }

    if constexpr (std::endian::native != std::endian::big) {
        return {
            std::make_move_iterator(std::rbegin(bytes)),
            std::make_move_iterator(std::rend(bytes))
        };
    }

    return bytes;
}
