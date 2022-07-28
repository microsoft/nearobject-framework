
#include "TlvSimple.hxx"

using namespace encoding;

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

    int datasize = data.size();

    if (datasize < TlvSimple::MinLengthWhenLessThanFF)
        return parseResult;
    tag = data[0];
    length = (uint16_t)data[1];
    if (length == 0xFF) {
        if (datasize < TlvSimple::MinLengthWhenMoreThanFF)
            return parseResult;
        length = ((uint16_t)data[2]) << 8;
        length += (uint16_t)data[3];
    }
    if (datasize != TlvSimple::MinLengthWhenMoreThanFF + length)
        return parseResult;
    auto tmpspan = data.last(length);
    value.assign(std::cbegin(tmpspan), std::cend(tmpspan));

    parseResult = Tlv::ParseResult::Succeeded;
    *tlvOutput = new TlvSimple(tag, value);

    return parseResult;
}

std::vector<std::byte>
TlvSimple::ToVector() const
{
    return m_value;
};

const void *
TlvSimple::get_tag() const
{
    return &m_tag;
}

const void *
TlvSimple::get_value() const
{
    return &m_value;
}
