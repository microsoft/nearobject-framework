
#ifndef TLV_SIMPLE_HXX
#define TLV_SIMPLE_HXX

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
    static constexpr auto OneByteLengthMinimumSize = 2;
    static constexpr auto ThreeByteLengthMinimumSize = 4;

    const std::vector<std::byte> m_tag;
    const std::vector<std::byte> m_value;

public:
    static constexpr auto ThreeByteLengthIndicatorValue = 0xFF;

    TlvSimple(std::byte tag, std::vector<std::byte> value);

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

#endif // TLV_SIMPLE_HXX
