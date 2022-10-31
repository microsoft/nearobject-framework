
#include <algorithm>
#include <bit>
#include <iterator>

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

bool
TlvBer::TagIsConstructed(std::span<const uint8_t> tag)
{
    return (tag.front() & BitmaskType) == TypeConstructed;
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

template<class D>
void
TlvBer::Builder::WriteLengthAndValue(const D& data)
{
    WriteLength(data.size());
    WriteBytes(data);
}

template<>
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
    if (m_validateConstructed && !TlvBer::TagIsConstructed(m_tag)) {
        throw InvalidTlvBerTagException();
    }
}
