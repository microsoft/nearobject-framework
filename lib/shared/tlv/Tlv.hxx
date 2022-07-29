
#ifndef __TLV_HXX__
#define __TLV_HXX__

#include <cstddef>
#include <memory>
#include <vector>

#include <gsl/span>

namespace encoding
{
class Tlv
{
public:
    gsl::span<const std::byte> Tag;
    gsl::span<const std::byte> Value;

    /**
     * @brief Convert this Tlv to a vector data blob. 
     * 
     * @return std::vector<std::byte> 
     */
    virtual std::vector<std::byte>
    ToVector() const = 0;

    /**
     * @brief Describes the result of parsing a Tlv
     * 
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

#endif // __TLV_HXX__
