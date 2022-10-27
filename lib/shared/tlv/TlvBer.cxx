
#include <algorithm>
#include <bit>
#include <iterator>

#include "TlvBer.hxx"

using namespace encoding;

TlvBer::TlvBer(const std::vector<uint8_t>& tag, const std::vector<uint8_t>& value) :
    m_tag(tag),
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
    if (m_validateConstructed && !TlvBer::TagIsConstructed(m_tag)) {
        throw InvalidTlvBerTagException();
    }
}
