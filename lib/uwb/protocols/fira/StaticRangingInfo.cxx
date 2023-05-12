
#include <iomanip>
#include <sstream>

#include <notstd/utility.hxx>
#include <tlv/TlvSerialize.hxx>
#include <uwb/protocols/fira/StaticRangingInfo.hxx>

using namespace uwb::protocol::fira;

std::string
StaticRangingInfo::ToString() const
{
    std::ostringstream staticRangingInfoString{};
    staticRangingInfoString
        << "VendorId " << VendorId
        << ", InitializationVector 0x";
    for (const auto& i : InitializationVector) {
        staticRangingInfoString << std::hex << std::setw(2) << std::setfill('0') << std::showbase << std::internal << +i;
    }

    return staticRangingInfoString.str();
}

std::unique_ptr<encoding::TlvBer>
StaticRangingInfo::ToDataObject() const
{
    using encoding::TlvBer, encoding::GetBytesBigEndianFromBitMap;

    auto tlvBerResult = std::make_unique<TlvBer>();
    *tlvBerResult = TlvBer::Builder()
        .SetTag(Tag)
        // VENDOR_ID
        .AddTlv(
            TlvBer::Builder()
                .SetTag(notstd::to_underlying(ParameterTag::VendorId))
                .SetValue(GetBytesBigEndianFromBitMap(VendorId, sizeof VendorId))
                .Build())
        // STATIC_STS_IV 
        .AddTlv(
            TlvBer::Builder()
                .SetTag(notstd::to_underlying(ParameterTag::StaticStsIv))
                .SetValue(InitializationVector)
                .Build())
        .Build();

    return tlvBerResult;
}

/* static */
StaticRangingInfo
StaticRangingInfo::FromDataObject(const encoding::TlvBer& tlv)
{
    // TODO
    return {};
}