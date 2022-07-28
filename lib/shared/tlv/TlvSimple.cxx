
#include "TlvSimple.hxx"

using namespace encoding;

/* static */
Tlv::ParseResult
TlvSimple::Parse(std::unique_ptr<Tlv>* tlvOutput, gsl::span<std::byte> data)
{
    std::unique_ptr<Tlv> tlvParsed{ nullptr };
    Tlv::ParseResult parseResult = Tlv::ParseResult::Failed;

    std::byte tag;
    int length;
    std::vector<std::byte> value;

    int datasize = data.size()

    if (datasize < 2) return parseResult;
    tag = data[0];
    length = data[1];
    if(length == 0xFF) {
        if (datasize < 4) return parseResult;
        length = (data[2] << 8) + data[3];
    }
    if (datasize != 4+length) return parseResult;
    value = data.last(length);

    if (parseResult == Tlv::ParseResult::Succeeded) {
        *tlvOutput = std::move(tlvParsed);
    }

    return parseResult;
}
