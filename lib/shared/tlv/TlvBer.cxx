
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

/* static */
Tlv::ParseResult
TlvBer::Parse(TlvBer **tlvOutput, const std::span<uint8_t>& data)
{
    if (!tlvOutput) {
        return Tlv::ParseResult::Failed;
    }

    *tlvOutput = nullptr;
    auto dataIt = std::cbegin(data);
    auto parseResult = Tlv::ParseResult::Failed;

    // Parse tag.
    std::vector<uint8_t> tag;

    // Is tag short type?
    if ((*dataIt & BitmaskTagFirstByte) != TagValueLongField) {
        tag.push_back(*dataIt & BitmaskTagFirstByte);
    // Tag is long-type. 
    } else {
        do {
            std::advance(dataIt, 1);
            tag.push_back(*dataIt & BitmaskTagLong);
        } while ((*dataIt & BitmaskTagLastByte) != TagValueLastByte);
    }

    // Parse length.
    std::advance(dataIt, 1);
    std::size_t length = 0;

    // Is length short form?
    if ((*dataIt & BitmaskLengthForm) == LengthFormShort) {
        length = *dataIt & BitmaskLengthShort;
    // Length is long form.
    } else {
        auto numOctets = *dataIt & BitmaskLengthNumOctets;
        for (auto i = 0; i < numOctets; i++, std::advance(dataIt, 1)) {
            length  = length << 8U;
            length |= *dataIt;
        }
    }

    // Parse value.
    std::advance(dataIt, 1);
    std::vector<uint8_t> value{ dataIt, std::next(dataIt, length) };
    switch (GetTagType(tag)) {
    case Type::Constructed:
        // TODO: parse nested tlvs
        break;
    case Type::Primitive:
        auto tlv = std::make_unique<TlvBer>(tag, value);
        *tlvOutput = tlv.release();
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
