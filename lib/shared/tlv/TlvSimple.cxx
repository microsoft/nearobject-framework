
#include <limits>

#include "TlvSimple.hxx"

using namespace encoding;

TlvSimple::TlvSimple(uint8_t tag, std::vector<uint8_t> value) :
    m_tag(1, tag),
    m_value(std::move(value))
{
    Tlv::Tag = std::span(m_tag);
    Tlv::Value = std::span(m_value);
}

/* static */
Tlv::ParseResult
TlvSimple::Parse(TlvSimple **tlvOutput, const std::span<uint8_t> &data)
{
    Tlv::ParseResult parseResult = Tlv::ParseResult::Failed;
    if (!tlvOutput) {
        return parseResult;
    }
    *tlvOutput = nullptr;

    uint8_t tag{};
    uint16_t length = 0;
    std::vector<uint8_t> value;

    const auto datasize = data.size();
    if (datasize < TlvSimple::OneByteLengthMinimumSize) {
        return parseResult;
    }
    tag = data[0];
    length = (uint8_t)data[1];
    if (length == TlvSimple::ThreeByteLengthIndicatorValue) {
        if (datasize < TlvSimple::ThreeByteLengthMinimumSize) {
            return parseResult;
        }
        length = (uint8_t)data[2];
        length <<= (std::size_t)std::numeric_limits<uint8_t>::digits;
        length |= (uint8_t)data[3];
        if (datasize != TlvSimple::ThreeByteLengthMinimumSize + length) {
            return parseResult;
        }
    } else if (datasize != TlvSimple::OneByteLengthMinimumSize + length) {
        return parseResult;
    }
    auto tmpspan = data.last(length);
    value.assign(std::cbegin(tmpspan), std::cend(tmpspan));

    parseResult = Tlv::ParseResult::Succeeded;
    auto tlvSimple = std::make_unique<TlvSimple>(tag, std::move(value));
    *tlvOutput = tlvSimple.release();

    return parseResult;
}
