
#include <array>
#include <cstring>
#include <stdexcept>

#include <notstd/type_traits.hxx>
#include <tlv/TlvSerialize.hxx>
#include <uwb/protocols/fira/UwbSessionData.hxx>

using namespace uwb::protocol::fira;

std::unique_ptr<encoding::TlvBer>
UwbSessionData::ToDataObject() const
{
    using encoding::TlvBer, encoding::GetBytesBigEndianFromBitMap;

    // Construct the tag data.
    std::array<uint8_t, sizeof Tag> tagData;
    std::memcpy(&tagData, &Tag, sizeof Tag);

    auto builder = TlvBer::Builder()
        .SetTag(tagData)
        // UWB_SESSION_DATA_VERSION
        .AddTlv(
            TlvBer::Builder()
                .SetTag(notstd::to_underlying(ParameterTag::SessionDataVersion))
                .SetValue(GetBytesBigEndianFromBitMap(sessionDataVersion, sizeof sessionDataVersion))
                .Build())
        // UWB_SESSION_ID
        .AddTlv(
            TlvBer::Builder()
                .SetTag(notstd::to_underlying(ParameterTag::SessionId))
                .SetValue(GetBytesBigEndianFromBitMap(sessionId, sizeof sessionId))
                .Build())
        // UWB_SUB_SESSION_ID
        .AddTlv(
            TlvBer::Builder()
                .SetTag(notstd::to_underlying(ParameterTag::SubSessionId))
                .SetValue(GetBytesBigEndianFromBitMap(subSessionId, sizeof subSessionId))
                .Build())
        // CONFIGURATION_PARAMETERS
        .AddTlv(*uwbConfiguration.ToDataObject())
        // UWB_CONFIG_AVAILABLE
        .AddTlv(
            TlvBer::Builder()
                .SetTag(notstd::to_underlying(ParameterTag::UwbConfigAvailable))
                .SetValue(0x01 /* always available since we send complete config? */)
                .Build());

    // STATIC_RANGING_INFO
    if (staticRangingInfo.has_value()) {
        builder.AddTlv(*staticRangingInfo->ToDataObject());
    }

    // SECURE_RANGING_INFO
    if (secureRangingInfo.has_value()) {
        builder.AddTlv(*secureRangingInfo->ToDataObject());
    }

    // REGULATORY_INFORMATION
    if (regulatoryInformation.has_value()) {
        builder.AddTlv(*regulatoryInformation->ToDataObject());
    }

    auto tlvBerResult = std::make_unique<TlvBer>();
    *tlvBerResult = builder.Build();

    return tlvBerResult;
}

/* static */
UwbSessionData
UwbSessionData::FromDataObject(const encoding::TlvBer& /* tlv */)
{
    throw std::logic_error("not implemented");
}
