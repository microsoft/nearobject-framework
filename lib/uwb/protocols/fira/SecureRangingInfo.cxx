
#include <notstd/utility.hxx>
#include <tlv/TlvSerialize.hxx>
#include <uwb/protocols/fira/SecureRangingInfo.hxx>

using namespace uwb::protocol::fira;

std::unique_ptr<encoding::TlvBer>
SecureRangingInfo::ToDataObject() const
{
    using encoding::TlvBer, encoding::GetBytesBigEndianFromBitMap;

    auto tlvBerResult = std::make_unique<TlvBer>();
    *tlvBerResult = TlvBer::Builder()
        .SetTag(Tag)
        .AddTlv(
            TlvBer::Builder()
                .SetTag(notstd::to_underlying(ParameterTag::UwbSessionKeyInfo))
                .SetValue(UwbSessionKeyInfo)
                .Build())
        .AddTlv(
            TlvBer::Builder()
                .SetTag(notstd::to_underlying(ParameterTag::ResponderSpecificSubSessionKeyInfo))
                .SetValue(ResponderSpecificSubSessionKeyInfo)
                .Build())
        .AddTlv(
            TlvBer::Builder()
                .SetTag(notstd::to_underlying(ParameterTag::SusAdditionalParameters))
                .SetValue(SusAdditionalParameters)
                .Build())
        .Build();

    return tlvBerResult;
}

/* static */
SecureRangingInfo
SecureRangingInfo::FromDataObject(const encoding::TlvBer& tlv)
{
    return {};
}