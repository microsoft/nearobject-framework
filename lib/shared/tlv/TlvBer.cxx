
#include <algorithm>
#include <bit>
#include <iterator>
#include <optional>
#include <utility>

#include "TlvBer.hxx"

using namespace encoding;

TlvBer::TlvBer(TlvBer::Class tlvClass, TlvBer::Type tlvType, const std::vector<uint8_t>& tagNumber, const std::vector<uint8_t>& tagComplete, const std::vector<uint8_t>& value) :
    m_class(tlvClass),
    m_type(tlvType),
    m_tagNumber(tagNumber),
    m_tag(tagComplete),
    m_value(value)
{
    ::Tlv::Tag = m_tag;
    ::Tlv::Value = m_value;
}

TlvBer::TlvBer(TlvBer::Class tlvClass, TlvBer::Type tlvType, const std::vector<uint8_t>& tagNumber, const std::vector<uint8_t>& tagComplete, std::vector<TlvBer>& values) :
    m_class(tlvClass),
    m_type(tlvType),
    m_tagNumber(tagNumber),
    m_tag(tagComplete),
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
    } else {
        return TlvBer::Type::Primitive;
    }
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
TlvBer::ParseTag(TlvBer::Class& tlvClass, TlvBer::Type& tlvType, std::vector<uint8_t>& tagNumber, std::vector<uint8_t>& tagComplete, uint8_t tag) {
    std::size_t bytesParsed = 0;
    const std::array<uint8_t, 1> tagArray{ tag };
    return ParseTag(tlvClass, tlvType, tagNumber, tagComplete, tagArray, bytesParsed);
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

std::span<const uint8_t>
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
TlvBer::ToBytes() const
{
    std::vector<uint8_t> accumulate;
    accumulate.assign(std::cbegin(m_tag), std::cend(m_tag));
    const auto lengthEncoding = TlvBer::GetLengthEncoding(m_value.size());
    accumulate.insert(std::cend(accumulate), std::cbegin(lengthEncoding), std::cend(lengthEncoding));
    accumulate.insert(std::cend(accumulate), std::cbegin(m_value), std::cend(m_value));
    return accumulate;
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

std::vector<uint8_t>
TlvBer::GetLengthEncoding(std::size_t length)
{
    for (const auto& [numBytes, maxValue, numOctetsInLengthFieldValue] : detail::LengthInfo) {
        if (length <= maxValue) {
            // Short-form, values 0-127.
            if (!numOctetsInLengthFieldValue.has_value()) {
                return { static_cast<uint8_t>(length) };
            }

            // Long-form, values 128+.
            std::vector<uint8_t> encoding(numBytes);
            for (auto i = numBytes-1; i != 0; i--) {
                encoding[i] = static_cast<uint8_t>(length & 0xFFU);
                length >>= 8U;
            }
            encoding[0] = numOctetsInLengthFieldValue.value();
            return encoding;
        }
    }

    throw std::length_error("length out of range");
}

template<size_t N>
void
TlvBer::Builder::WriteLengthAndValue(const std::array<const uint8_t, N>& data)
{
    WriteLength(data.size());
    WriteBytes(data);
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
TlvBer::Builder::AddTlv(const Tlv& tlv)
{
    return AddTlv(tlv.Tag, tlv.Value);
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
    return TlvBer{ m_class, m_type, m_tagNumber, m_tag, m_data };
}

void
TlvBer::Builder::ValidateTag()
{
    if (m_validateConstructed != (m_type == TlvBer::Type::Constructed)) {
        throw InvalidTlvBerTagException();
    }
}
