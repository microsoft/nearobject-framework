
#ifndef TLV_BER_HXX
#define TLV_BER_HXX

#include "Tlv.hxx"

#include <cstddef>
#include <vector>

namespace encoding
{
class TlvBer : public Tlv
{
public:
    /**
     * @brief Decode a Tlv from a blob of BER-TLV data.
     *
     * @param tlvOutput The decoded Tlv, if parsing was successful (ParseResult::Succeeded). This must be a writeable pointer.
     * @param data The data to parse a Tlv from.
     * @return ParseResult The result of the parsing operation.
     */
    static ParseResult
    Parse(TlvBer **tlvOutput, const gsl::span<std::byte>& data);
};

} // namespace encoding

#endif // TLV_BER_HXX
