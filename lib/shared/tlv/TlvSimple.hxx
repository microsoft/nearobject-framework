
#ifndef __TLV_SIMPLE_HXX__
#define __TLV_SIMPLE_HXX__

#include "Tlv.hxx"

#include <cstddef>
#include <memory>
#include <vector>

#include <gsl/span>

namespace encoding
{
class TlvSimple : public Tlv
{
private:
    std::byte m_tag;
    gsl::span<std::byte> m_value;

public:
    TlvSimple(std::byte tag, gsl::span<std::byte> value){
        m_tag = tag;
        m_value = value;
    }

    /**
     * @brief Convert this Tlv to a vector data blob. 
     * 
     * @return std::vector<std::byte> 
     */
    std::vector<std::byte>
    ToVector() const override;

    /**
     * @brief Decode a Tlv from a blob of SIMPLE-TLV data.
     *
     * @param tlvOutput The decoded Tlv, if parsing was successful (ParseResult::Succeeded). This must be a writeable pointer.
     * @param data The data to parse a Tlv from.
     * @return ParseResult The result of the parsing operation.
     */
    static ParseResult
    Parse(TlvSimple** tlvOutput, const gsl::span<std::byte>& data);
};

} // namespace encoding

#endif // __TLV_SIMPLE_HXX__
