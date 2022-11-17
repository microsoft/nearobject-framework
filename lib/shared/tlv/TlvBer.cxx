
#include <algorithm>
#include <bit>
#include <iterator>
#include <optional>
#include <utility>

#include "TlvBer.hxx"

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

using namespace encoding;

TlvBer::TlvBer(TlvBer::Class tlvClass, TlvBer::Type tlvType, uint32_t tagNumber, const std::vector<uint8_t>& tag, const std::vector<uint8_t>& value) :
    m_class(tlvClass),
    m_type(tlvType),
    m_tagNumber(tagNumber),
    m_tag(tag),
    m_value(value)
{
    ::Tlv::Tag = m_tag;
    ::Tlv::Value = m_value;
}

TlvBer::TlvBer(TlvBer::Class tlvClass, TlvBer::Type tlvType, uint32_t tagNumber, const std::vector<uint8_t>& tag, std::vector<TlvBer>& values) :
    m_class(tlvClass),
    m_type(tlvType),
    m_tagNumber(tagNumber),
    m_tag(tag),
    m_valuesConstructed(std::move(values))
{
    ::Tlv::Tag = m_tag;
    ::Tlv::Value = std::span<const uint8_t>{};
}

/* static */
TlvBer::TlvBer::Type
TlvBer::GetType(uint8_t tag)
{
    if ((tag & BitmaskType) == TypeConstructed) {
        return TlvBer::Type::Constructed;
    }
    return TlvBer::Type::Primitive;
}

/* static */
TlvBer::Class
TlvBer::GetClass(uint8_t tag)
{
    switch (tag & BitmaskClass) {
    case ClassUniversal:
        return TlvBer::Class::Universal;
    case ClassApplication:
        return TlvBer::Class::Application;
    case ClassContextSpecific:
        return TlvBer::Class::ContextSpecific;
    case ClassPrivate:
    default:
        return TlvBer::Class::Private;
    }
}

/* static */
Tlv::ParseResult
TlvBer::ParseTag(TlvBer::Class& tlvClass, TlvBer::Type& tlvType, uint32_t& tagNumber, std::vector<uint8_t>& tag, uint8_t tagValue)
{
    std::size_t bytesParsed = 0;
    const std::array<uint8_t, 1> tagArray{ tagValue };
    return ParseTag(tlvClass, tlvType, tagNumber, tag, tagArray, bytesParsed);
}

TlvBer::Type
TlvBer::GetType() const noexcept
{
    return m_type;
}

TlvBer::Class
TlvBer::GetClass() const noexcept
{
    return m_class;
}

uint32_t
TlvBer::GetTagNumber() const noexcept
{
    return m_tagNumber;
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
    return m_type == TlvBer::TlvBer::Type::Constructed;
}

bool
TlvBer::IsPrimitive() const noexcept
{
    return m_type == TlvBer::TlvBer::Type::Primitive;
}

std::vector<uint8_t>
// Static-analysis flags false positive as ToBytes() is called no another instance, thus is not actually recursive.
// NOLINTNEXTLINE(misc-no-recursion)
TlvBer::ToBytes() const
{
    std::vector<uint8_t> value;

    if (IsPrimitive()) {
        value = m_value;
    } else {
        for (const auto& tlv : m_valuesConstructed) {
            auto data = tlv.ToBytes();
            value.insert(std::cend(value), std::make_move_iterator(std::begin(data)), std::make_move_iterator(std::end(data)));
        }
    }

    const auto lengthEncoding = TlvBer::GetLengthEncoding(value.size());

    // Build the byte array.
    std::vector<uint8_t> accumulate;
    accumulate.insert(std::cend(accumulate), std::cbegin(m_tag), std::cend(m_tag));
    accumulate.insert(std::cend(accumulate), std::cbegin(lengthEncoding), std::cend(lengthEncoding));
    accumulate.insert(std::cend(accumulate), std::cbegin(value), std::cend(value));
    return accumulate;
}

Tlv::ParseResult
TlvBer::ParseConstructedValue(std::vector<TlvBer>& valueOutput, std::size_t length, std::span<uint8_t> dataInput, std::size_t& bytesParsedOverall)
{
    bytesParsedOverall = 0;
    std::size_t bytesParsed = 0;
    std::span<uint8_t> subspan = dataInput;

    while (bytesParsedOverall < length) {
        TlvBer subtlv{};
        auto parseResult = Parse(subtlv, subspan, bytesParsed);
        if (parseResult != Tlv::ParseResult::Succeeded) {
            return parseResult;
        }

        bytesParsedOverall += bytesParsed;
        subspan = dataInput.subspan(bytesParsedOverall);
        valueOutput.push_back(std::move(subtlv));
    }

    if (bytesParsedOverall == length) {
        return Tlv::ParseResult::Succeeded;
    }

    return Tlv::ParseResult::Failed;
}

Tlv::ParseResult
TlvBer::Parse(TlvBer& tlvOutput, std::span<uint8_t> dataInput, std::size_t& bytesParsedOverall)
{
    // Parse tag.
    auto tlvClass = TlvBer::Class::Invalid;
    auto tlvType = TlvBer::Type::Primitive;
    uint32_t tagNumber = 0;
    std::vector<uint8_t> tag;
    std::size_t offset = 0;
    std::size_t bytesParsed = 0;
    auto parseResult = ParseTag(tlvClass, tlvType, tagNumber, tag, dataInput, bytesParsed);
    if (parseResult != Tlv::ParseResult::Succeeded) {
        return parseResult;
    }

    // Parse length.
    offset += bytesParsed;
    std::size_t length = 0;
    auto subspan = dataInput.subspan(offset);
    parseResult = ParseLength(length, subspan, bytesParsed);
    if (parseResult != Tlv::ParseResult::Succeeded) {
        return parseResult;
    }

    // Parse value.
    offset += bytesParsed;
    subspan = dataInput.subspan(offset);
    if (tlvType == Type::Constructed) {
        std::vector<TlvBer> values;
        parseResult = ParseConstructedValue(values, length, subspan, bytesParsed);
        if (parseResult != Tlv::ParseResult::Succeeded) {
            return parseResult;
        }
        tlvOutput = TlvBer(tlvClass, tlvType, tagNumber, tag, values);
    } else {
        std::vector<uint8_t> value;
        parseResult = ParsePrimitiveValue(value, length, subspan, bytesParsed);
        if (parseResult != Tlv::ParseResult::Succeeded) {
            return parseResult;
        }
        tlvOutput = TlvBer(tlvClass, tlvType, tagNumber, tag, value);
    }

    offset += bytesParsed;
    bytesParsedOverall = offset;

    return ParseResult::Succeeded;
}

void
TlvBer::Builder::WriteLength(uint64_t length)
{
    const auto lengthEncoding = TlvBer::GetLengthEncoding(length);
    m_data.insert(std::cend(m_data), std::cbegin(lengthEncoding), std::cend(lengthEncoding));
}

TlvBer::Builder&
TlvBer::Builder::SetTag(uint8_t tag)
{
    TlvBer::ParseTag(m_class, m_type, m_tagNumber, m_tag, tag);
    return *this;
}

std::vector<uint8_t>
TlvBer::GetLengthEncoding(std::size_t length)
{
    // Short-form, values 0-127.
    if (length <= LengthFormShortMax) {
        return { static_cast<uint8_t>(length) };
    }

    // Long-form, values 128+.
    std::size_t numBytes = 0;
    std::vector<uint8_t> encoding;

    // Successively encode length value with big endian byte ordering.
    while (length > 0) {
        encoding.push_back(length & 0xFFU);
        length >>= 8U;
        numBytes++;
    }

    // Encode the long-format indicator and number of trailing bytes.
    encoding.push_back((numBytes & BitmaskLengthNumOctets) | LengthFormLong);

    // Ensure returned encoding is big-endian, accounting for host endianness.
    if constexpr (std::endian::native == std::endian::big) {
        return encoding;
    } else {
        return {
            std::make_move_iterator(std::rbegin(encoding)),
            std::make_move_iterator(std::rend(encoding))
        };
    }
}

void
TlvBer::Builder::WriteLengthAndValue(std::span<const uint8_t> data)
{
    WriteLength(data.size());
    WriteBytes(data);
}

void
TlvBer::Builder::WriteLengthAndValue(uint8_t value)
{
    WriteLength(uint8_t(1));
    m_data.push_back(value);
}

TlvBer::Builder&
TlvBer::Builder::SetValue(uint8_t value)
{
    m_data.assign(1, value);
    return *this;
}

TlvBer::Builder&
TlvBer::Builder::AddTlv(const TlvBer& tlv)
{
    m_valuesConstructed.push_back(tlv);
    return *this;
}

TlvBer::Builder&
TlvBer::Builder::SetAsCopyOfTlv(const TlvBer& tlv)
{
    m_class = tlv.m_class;
    m_type = tlv.m_type;
    m_tagNumber = tlv.m_tagNumber;
    m_tag = tlv.m_tag;
    m_data = tlv.m_value;
    m_valuesConstructed = tlv.m_valuesConstructed;
    return *this;
}

TlvBer::Builder&
TlvBer::Builder::Reset()
{
    *this = {};
    return *this;
}

TlvBer
TlvBer::Builder::Build()
{
    ValidateTag();
    if (m_type == TlvBer::Type::Primitive) {
        return TlvBer{ m_class, m_type, m_tagNumber, m_tag, m_data };
    }
    return TlvBer{ m_class, m_type, m_tagNumber, m_tag, m_valuesConstructed };
}

void
TlvBer::Builder::ValidateTag()
{
    if ((m_type == TlvBer::Type::Constructed) && (!m_data.empty()) ||
        (m_type == TlvBer::Type::Primitive) && (!m_valuesConstructed.empty())) {
        throw InvalidTlvBerTagException();
    }
}

bool
TlvBer::operator==(const TlvBer& other) const
{
    return std::tie(m_class, m_type, m_tagNumber, m_tag, m_value, m_valuesConstructed) == std::tie(other.m_class, other.m_type, other.m_tagNumber, other.m_tag, other.m_value, other.m_valuesConstructed);
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
