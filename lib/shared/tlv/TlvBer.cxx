
#include <algorithm>
#include <bit>
#include <iterator>
#include <optional>
#include <utility>

#include "TlvBer.hxx"

using namespace encoding;

TlvBer::TlvBer(const std::vector<uint8_t>& tag, const std::vector<uint8_t>& length, const std::vector<uint8_t>& value) :
    m_tag(tag),
    m_length(length),
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

TlvBer::Type
TlvBer::GetType() const noexcept
{
    return GetTagType(Tlv::Tag);
}

std::vector<uint8_t>
TlvBer::ToBytes()
{
    std::vector<uint8_t> accumulate;
    accumulate.assign(std::cbegin(m_tag), std::cend(m_tag));
    accumulate.insert(std::cend(accumulate), std::cbegin(m_length), std::cend(m_length));
    accumulate.insert(std::cend(accumulate), std::cbegin(m_value), std::cend(m_value));
    return accumulate;
}

void
TlvBer::Builder::WriteLength(uint64_t length)
{
    std::vector<uint8_t> lengthEncoding = CalculateLengthEncoding(length);
    m_data.insert(std::cend(m_data), std::cbegin(lengthEncoding), std::cend(lengthEncoding));
}

std::vector<uint8_t>
TlvBer::Builder::CalculateLengthEncoding(uint8_t length)
{
    if (length > 127)
        return CalculateLengthEncoding(uint64_t(length));
    else
        return std::vector<uint8_t>(1, length);
}

std::vector<uint8_t>
TlvBer::Builder::CalculateLengthEncoding(uint64_t length)
{
    if (length <= 127)
        return CalculateLengthEncoding(uint8_t(length));

    std::vector<uint8_t> holder;

    holder.push_back(0x88);

    // std::vector<const uint8_t> bytesInBigEndian;

    for (int i = 0; i < 8; i++) {
        const auto b = static_cast<uint8_t>((length & 0xFF00'0000'0000'0000) >> 56);
        holder.push_back(b);
        // bytesInBigEndian.push_back(b);
        length <<= 8;
    }
    return holder;
}

TlvBer::Builder&
TlvBer::Builder::SetTag(const std::span<const uint8_t>& tag)
{
    m_tag.assign(std::cbegin(tag), std::cend(tag));
    return *this;
}

template<size_t N>
TlvBer::Builder&
TlvBer::Builder::SetTag(const std::array<uint8_t, N>& tag)
{
    m_tag.assign(std::cbegin(tag), std::cend(tag));
    return *this;
}

TlvBer::Builder&
TlvBer::Builder::SetTag(const uint8_t& tag)
{
    m_tag.assign(1, tag);
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

template<size_t N>
void
TlvBer::Builder::WriteLengthAndValue(const std::array<const uint8_t, N>& data)
{
    WriteLength(data.size());
    WriteBytes(data);
}

void
TlvBer::Builder::WriteLengthAndValue(const std::span<const uint8_t>& data)
{
    WriteLength(data.size());
    WriteBytes(data);
}

void
TlvBer::Builder::WriteLengthAndValue(const uint8_t& value)
{
    WriteLength(uint8_t(1));
    m_data.push_back(value);
}

template<size_t N>
TlvBer::Builder&
TlvBer::Builder::SetValue(const std::array<const uint8_t, N>& data)
{
    m_data.assign(std::cbegin(data), std::cend(data));
    return *this;
}

TlvBer::Builder&
TlvBer::Builder::SetValue(const std::span<const uint8_t>& data)
{
    m_data.assign(std::cbegin(data), std::cend(data));
    return *this;
}

TlvBer::Builder&
TlvBer::Builder::SetValue(const uint8_t& value)
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
    m_validateConstructed = false;
    m_tag.clear();
    m_data.clear();
    return *this;
}

TlvBer
TlvBer::Builder::Build()
{
    ValidateTag();
    CalculateAndSetLength();
    return TlvBer{ m_tag, m_length, m_data };
}

void
TlvBer::Builder::CalculateAndSetLength()
{
    m_length = CalculateLengthEncoding(m_data.size());
}

void
TlvBer::Builder::ValidateTag()
{
    if (m_validateConstructed && TlvBer::GetTagType(m_tag) != Type::Constructed) {
        throw InvalidTlvBerTagException();
    }
}
