
#include "TlvSimple.hxx"

using namespace encoding;

/* static */
Tlv::ParseResult
TlvSimple::Parse(TlvSimple** tlvOutput, const gsl::span<std::byte>& data)
{
    Tlv::ParseResult parseResult = Tlv::ParseResult::Failed;
    if (!tlvOutput)
        return parseResult;
    *tlvOutput = nullptr;

    std::byte tag;
    int length;
    gsl::span<std::byte> value;

    int datasize = data.size();

    if (datasize < 2)
        return parseResult;
    tag = data[0];
    length = (int)data[1];
    if (length == 0xFF) {
        if (datasize < 4)
            return parseResult;
        length = ((int)data[2]) << 8;
        length += (int)data[3];
    }
    if (datasize != 4 + length)
        return parseResult;
    value = data.last(length);

    parseResult = Tlv::ParseResult::Succeeded;
    *tlvOutput = new TlvSimple(tag, value);

    return parseResult;
}
