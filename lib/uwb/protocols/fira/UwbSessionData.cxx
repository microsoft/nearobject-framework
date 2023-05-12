
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

    TlvBer::Builder builder{};

    builder
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
        .AddTlv(*uwbConfiguration.ToDataObject());

    // The below tags being optional, we need to determine if when they are not
    // present/set, whether: the TLV:
    //      1) Can be wholly omitted (current behavior), or
    //      2) Must be present but with a length field of 0.
    // This won't matter when interoperating with other devices running this code, but it will
    // matter for inter-operability.

    // STATIC_RANGING_INFO
    if (staticRangingInfo.has_value()) {
        builder.AddTlv(*staticRangingInfo->ToDataObject());
    }
    // SECURE_RANGING_INFO
    if (secureRangingInfo.has_value()) {
        builder.AddTlv(*secureRangingInfo->ToDataObject());
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
