
#ifndef TLV_HXX
#define TLV_HXX

#include <cstddef>
#include <memory>
#include <span>
#include <vector>

namespace encoding
{
class Tlv
{
public:
    std::span<const std::byte> Tag;
    std::span<const std::byte> Value;

    /**
     * @brief Describes the result of parsing a Tlv
     */
    enum class ParseResult {
        /**
         * @brief Parsing succeeded and a valid Tlv was produced.
         */
        Succeeded,

        /**
         * @brief Parsing failed and no valid Tlv was produced. Note that this
         * implies a Tlv was found, but it could not be successfully parsed.
         */
        Failed,

        /**
         * @brief No Tlv was found in the specified input, thus parsing was not
         * attempted.
         */
        TlvNotFound,

        /**
         * @brief An unknown error occurred. Nothing may be assumed about the
         * content of the input data.
         */
        UnknownError,
    };
};

} // namespace encoding

#endif // TLV_HXX
