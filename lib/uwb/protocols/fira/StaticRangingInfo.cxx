
#include <algorithm>
#include <array>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <vector>

#include <magic_enum.hpp>
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
StaticRangingInfo::FromDataObject(const encoding::TlvBer& tlvBer)
{
    using encoding::ReadSizeTFromBytesBigEndian;

    StaticRangingInfo staticRangingInfo{};
    std::vector<encoding::TlvBer> tlvBerValues = tlvBer.GetValues();

    for (const auto &tlvBerValue : tlvBerValues) {
        auto tagValue = tlvBerValue.GetTag();
        // All tags for StaticRangingInfo are 1-byte long, so ignore all others.
        if (std::size(tagValue) != 1) {
            continue;
        }

        // Ensure the tag has a corresponding enumeration value.
        auto parameterTag = magic_enum::enum_cast<ParameterTag>(tagValue.front());
        if (!parameterTag.has_value()) {
            continue;
        }

        // Ensure all values have non-zero payload.
        auto& parameterValue = tlvBerValue.GetValue();
        if (std::empty(parameterValue)) {
            continue;
        }

        switch (*parameterTag) {
        case ParameterTag::VendorId: {
            staticRangingInfo.VendorId = ReadSizeTFromBytesBigEndian<decltype(staticRangingInfo.VendorId)>(parameterValue);
            break;
        }
        case ParameterTag::StaticStsIv: {
            std::memcpy(std::data(staticRangingInfo.InitializationVector), std::data(parameterValue), std::min(std::size(parameterValue), std::size(staticRangingInfo.InitializationVector)));
            break;
        }
        default: {
            break;
        }
        }
    }

    return staticRangingInfo;
}
