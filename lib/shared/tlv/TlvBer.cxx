
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
TlvBer::Builder::WriteLength(uint64_t length)
{
    std::vector<const uint8_t> bytesInLittleEndian ;

    for(int i=0;i<8;i++){
        const auto b = static_cast<uint8_t>((length & 0xFF));
        bytesInLittleEndian.push_back(b);
        length >>= 8;
    }
    
    // write the bytes in big endian 
    m_data.insert(std::cend(m_data), std::rbegin(bytesInLittleEndian), std::rend(bytesInLittleEndian));
}

template <typename IterableOfBytes>
TlvBer::Builder&
TlvBer::Builder::SetTag(const IterableOfBytes& tag)
{
    m_tag.assign(std::cbegin(tag), std::cend(tag));
    return *this;
}

template<>
TlvBer::Builder&
TlvBer::Builder::SetTag(const uint8_t& tag)
{
    m_tag.assign(1, tag);
    return *this;
}

template <class D>
void
TlvBer::Builder::WriteLengthAndValue(const D& data){
    WriteLength(data.size());
    WriteBytes(data);
}

template <>
void
TlvBer::Builder::WriteLengthAndValue(const uint8_t& value){
    WriteLength(1);
    m_data.push_back(value);
}

TlvBer::Builder&
TlvBer::Builder::AddTlv(const Tlv& tlv)
{
    return AddTlv(tlv.Tag, tlv.Value);
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
