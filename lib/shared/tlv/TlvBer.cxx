
#include <algorithm>
#include <bit>
#include <iterator>
#include <optional>
#include <utility>

#include "TlvBer.hxx"

using namespace encoding;

TlvBer::TlvBer(const std::vector<uint8_t>& tag, const std::vector<uint8_t>& value) :
    m_tag(tag),
    m_value(value)
{
    ::Tlv::Tag = m_tag;
    ::Tlv::Value = m_value;
}

TlvBer::TlvBer(const std::vector<uint8_t>& tag, std::vector<TlvBer> values) :
    m_tag(tag),
    m_valuesConstructed(std::move(values))
{
    ::Tlv::Tag = m_tag;
    ::Tlv::Value = std::span<const uint8_t>{};
}

/* static */
TlvBer::Type
TlvBer::GetTagType(std::span<const uint8_t> tag)
{
    if ((tag.front() & BitmaskType) == TypeConstructed) {
        return Type::Constructed;
    } else {
        return Type::Primitive;
    }
}

/* static */
TlvBer::Class
TlvBer::GetClass(std::span<const uint8_t> tag)
{
    switch (tag.front() & BitmaskClass) {
    case ClassUniversal:
        return Class::Universal;
    case ClassApplication:
        return Class::Application;
    case ClassContextSpecific:
        return Class::ContextSpecific;
    case ClassPrivate:
    default:
        return Class::Private;
    }
}

Tlv::ParseResult
TlvBer::ParseTag(std::vector<uint8_t>& tagOutput, std::span<uint8_t>::iterator& dataIt, std::span<uint8_t>::iterator dataEnd){
    if(dataIt==dataEnd) return Tlv::ParseResult::Failed;

    tagOutput.push_back(*dataIt);

    // Is tag short type?
    if ((*dataIt & BitmaskTagFirstByte) != TagValueLongField) {
        return Tlv::ParseResult::Succeeded;
    } else {
        // Tag is long-type. 
        do {
            dataIt++;
            if(dataIt==dataEnd) return Tlv::ParseResult::Failed;
            tagOutput.push_back(*dataIt);
        } while ((*dataIt & BitmaskTagLastByte) != TagValueLastByte);
    }
    dataIt++; // advance the dataIt to once past the tag
    return Tlv::ParseResult::Succeeded;
}

Tlv::ParseResult
TlvBer::ParseLength(size_t& length, std::span<uint8_t>::iterator& dataIt, std::span<uint8_t>::iterator dataEnd){
    if(dataIt==dataEnd) return Tlv::ParseResult::Failed;

    // Is length short form?
    if ((*dataIt & BitmaskLengthForm) == LengthFormShort) {
        length = *dataIt & BitmaskLengthShort;
    } else {
        // Length is long form.
        auto numOctets = *dataIt & BitmaskLengthNumOctets;
        if(numOctets > MaxNumOctetsInLengthEncoding) return Tlv::ParseResult::Failed;

        for (auto i = 0; i < numOctets; i++) {
            dataIt++;
            if(dataIt==dataEnd) return Tlv::ParseResult::Failed;

            length  = length << 8U;
            length |= *dataIt;
        }
    }
    dataIt++; // advance the dataIt to once past the lengthEncoding

    return Tlv::ParseResult::Succeeded;
}

Tlv::ParseResult
TlvBer::ParseValue(std::vector<uint8_t>& valueOutput,size_t length, std::span<uint8_t>::iterator& dataIt, std::span<uint8_t>::iterator dataEnd){
    if(dataEnd - dataIt < length) return Tlv::ParseResult::Failed;
    valueOutput = std::vector<uint8_t> { dataIt, std::next(dataIt, length) };
    return Tlv::ParseResult::Succeeded;
}

/* static */
template<typename Iterable>
Tlv::ParseResult
TlvBer::Parse(TlvBer **tlvOutput, Iterable& dataInput)
{
    if (!tlvOutput) {
        return Tlv::ParseResult::Failed;
    }

    std::span<uint8_t> data {std::cbegin(dataInput),std::cend(dataInput)};

    *tlvOutput = nullptr;
    auto dataIt = std::cbegin(data);
    auto dataEnd = std::cend(data);
    auto parseResult = Tlv::ParseResult::Failed;

    // Parse tag.
    std::vector<uint8_t> tag;
    parseResult = ParseTag(tag,dataIt,dataEnd);
    if(Tlv::ParseResult::Succeeded != parseResult) return parseResult;

    // Parse length.
    std::size_t length = 0;
    parseResult = ParseLength(length,dataIt,dataEnd);
    if(Tlv::ParseResult::Succeeded != parseResult) return parseResult;

    // Parse value.
    std::vector<uint8_t> value;
    parseResult = ParseValue(value,length,dataIt,dataEnd);
    if(Tlv::ParseResult::Succeeded != parseResult) return parseResult;

    switch (GetTagType(tag)) {
    case Type::Constructed:
        // TODO: parse nested tlvs
        break;
    case Type::Primitive:
        *tlvOutput = std::make_unique<TlvBer>(tag, value).release();
        parseResult = ParseResult::Succeeded;
        break;
    }

    return parseResult;
}

std::span<const uint8_t>
TlvBer::GetTag() const noexcept
{
    return m_tag;
}

std::vector<TlvBer>
TlvBer::GetValues() const noexcept
{
    return m_valuesConstructed;
}

bool
TlvBer::IsConstructed() const noexcept
{
    return GetType() == TlvBer::Type::Constructed;
}

bool
TlvBer::IsPrimitive() const noexcept
{
    return GetType() == TlvBer::Type::Primitive;
}

void 
TlvBer::Builder::WriteData(uint32_t data)
{
    const auto b0 = static_cast<uint8_t>((data & 0xFF000000U) >> 24U);
    const auto b1 = static_cast<uint8_t>((data & 0x00FF0000U) >> 16U);
    const auto b2 = static_cast<uint8_t>((data & 0x0000FF00U) >>  8U);
    const auto b3 = static_cast<uint8_t>((data & 0x000000FFU) >>  0U);

    if constexpr (std::endian::native == std::endian::little) {
        m_data.push_back(b3);
        m_data.push_back(b2);
        m_data.push_back(b1);
        m_data.push_back(b0);
    } else {
        m_data.push_back(b0);
        m_data.push_back(b1);
        m_data.push_back(b2);
        m_data.push_back(b3);
    }
}

namespace detail
{
const std::array<std::tuple<uint8_t, std::size_t, std::optional<uint8_t>>, 5> LengthInfo = {
    std::make_tuple(1, 0x0000007FU, std::nullopt),
    std::make_tuple(2, 0x000000FFU, TlvBer::LengthTag2Byte),
    std::make_tuple(3, 0x0000FFFFU, TlvBer::LengthTag3Byte),
    std::make_tuple(4, 0x00FFFFFFU, TlvBer::LengthTag4Byte),
    std::make_tuple(5, 0xFFFFFFFFU, TlvBer::LengthTag5Byte)
};
} // namespace detail

/* static */
std::vector<uint8_t>
TlvBer::GetLengthEncoding(std::size_t length)
{
    for (const auto& [numBytes, maxValue, numOctetsInLengthFieldValue]: detail::LengthInfo) {
        if (length <= maxValue) {
            // Short-form, values 0-127.
            if (!numOctetsInLengthFieldValue.has_value()) {
                return { static_cast<uint8_t>(length) };
            }

            // Long-form, values 128+.
            std::vector<uint8_t> encoding(numBytes);
            for (auto i = numBytes; numBytes > 0; i--) {
                encoding[i] = static_cast<uint8_t>(length & 0xFFU);
                length >>= 8U;
            }
            encoding[0] = numOctetsInLengthFieldValue.value();
            return encoding;
        }
    }

    throw std::length_error("length out of range");        
}

void
TlvBer::Builder::WriteData(std::span<const uint8_t> data)
{
    m_data.insert(std::cend(m_data), std::cbegin(data), std::cend(data));
}

void
TlvBer::Builder::WriteValue(uint8_t value)
{
    m_data.push_back(1);
    m_data.push_back(value);
}

void
TlvBer::Builder::WriteValue(std::span<const uint8_t> value)
{
    WriteData(value.size_bytes());
    WriteData(value);
}

TlvBer::Builder&
TlvBer::Builder::SetTag(uint8_t tag)
{
    m_tag.assign(1, tag);
    return *this;
}

TlvBer::Builder&
TlvBer::Builder::AddTlv(auto&& func)
{
    func();
    m_validateConstructed = true;
    return *this;
}

TlvBer::Builder&
TlvBer::Builder::AddTlv(const Tlv& tlv)
{
    return AddTlv([&]{
        WriteData(tlv.Tag);
        WriteValue(tlv.Value);
    });
}

TlvBer 
TlvBer::Builder::Build()
{
    ValidateTag();
    return TlvBer{ m_tag, m_data };
}

void
TlvBer::Builder::ValidateTag()
{
    if (m_validateConstructed && TlvBer::GetTagType(m_tag) != Type::Constructed) {
        throw InvalidTlvBerTagException();
    }
}
