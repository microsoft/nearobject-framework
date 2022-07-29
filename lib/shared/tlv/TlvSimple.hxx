
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
    static const int OneByteLengthMinimumSize = 2;
    static const int ThreeByteLengthMinimumSize = 4;

    const std::vector<std::byte> _m_tag;
    const std::vector<std::byte> _m_value;

public:
    TlvSimple(std::byte tag, const std::vector<std::byte> &value) :
        _m_tag(1,tag),
        _m_value(value)
    {
        // Tlv::m_tag = gsl::span<std::byte>{std::data(_m_tag),std::size(_m_tag)};
        Tlv::m_tag = gsl::span<std::byte>(std::data(_m_tag),std::size(_m_tag));
        // Tlv::m_value = gsl::span<std::byte>{std::data(_m_value),std::size(_m_value)};
        Tlv::m_value = gsl::span<std::byte>(std::data(_m_value),std::size(_m_value));
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
    Parse(TlvSimple **tlvOutput, const gsl::span<std::byte> &data);
};

} // namespace encoding

#endif // __TLV_SIMPLE_HXX__
