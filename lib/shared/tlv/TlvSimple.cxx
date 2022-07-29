
#include "TlvSimple.hxx"

using namespace encoding;

TlvSimple::TlvSimple(std::byte tag, std::vector<std::byte> value) :
    m_tag(1, tag),
    m_value(value)
{
    Tlv::Tag = gsl::span(m_tag);
    Tlv::Value = gsl::span(m_value);
}

/* static */
Tlv::ParseResult
TlvSimple::Parse(TlvSimple **tlvOutput, const gsl::span<std::byte> &data)
{
    Tlv::ParseResult parseResult = Tlv::ParseResult::Failed;
    if (!tlvOutput)
        return parseResult;
    *tlvOutput = nullptr;

    std::byte tag;
    uint16_t length;
    std::vector<std::byte> value;

    auto datasize = data.size();

    if (datasize < TlvSimple::OneByteLengthMinimumSize)
        return parseResult;
    tag = data[0];
    length = (uint8_t)data[1];
    if (length == TlvSimple::ThreeByteLengthIndicatorValue) {
        if (datasize < TlvSimple::ThreeByteLengthMinimumSize)
            return parseResult;
        length = (uint8_t)data[2];
        length <<= 8;
        length |= (uint8_t)data[3];
    }
    if (datasize != TlvSimple::ThreeByteLengthMinimumSize + length)
        return parseResult;
    auto tmpspan = data.last(length);
    value.assign(std::cbegin(tmpspan), std::cend(tmpspan));

    parseResult = Tlv::ParseResult::Succeeded;
    *tlvOutput = new TlvSimple(tag, value);

    return parseResult;
}