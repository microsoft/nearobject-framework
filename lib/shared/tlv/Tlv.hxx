
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
    /**
     * @brief Convert this Tlv to a vector data blob. 
     * 
     * @return std::vector<std::byte> 
     */
    virtual std::vector<std::byte>
    ToVector() const = 0;

    /**
     * @brief Describes the result of parsing a Tlv using the Parse() function. 
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

    /**
     * @brief Decode a Tlv from a blob of data.
     *
     * @param tlvOutput The decoded Tlv, if parsing was successful (ParseResult::Succeeded).
     * @param data The data to parse a Tlv from.
     * @return ParseResult The result of the parsing operation.
     */
    static ParseResult
    Parse(std::unique_ptr<Tlv>* tlvOutput, gsl::span<std::byte> data);
};

using TlvSimple = Tlv;

} // namespace encoding

#endif // __TLV_HXX__
