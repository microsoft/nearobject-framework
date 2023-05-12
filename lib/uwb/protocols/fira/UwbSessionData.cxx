
#include <array>
#include <cstring>
#include <stdexcept>

#include <magic_enum.hpp>
#include <notstd/type_traits.hxx>
#include <tlv/TlvSerialize.hxx>
#include <uwb/protocols/fira/UwbSessionData.hxx>
#include <uwb/protocols/fira/UwbException.hxx>

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
                .SetValue(static_cast<uint8_t>(uwbConfigurationAvailable))
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
UwbSessionData::FromDataObject(const encoding::TlvBer& tlvBer)
{
    static const std::unordered_set<ParameterTag> parameterTagsRequired{
        ParameterTag::SessionDataVersion,
        ParameterTag::SessionId,
        ParameterTag::SubSessionId,
        ParameterTag::ConfigurationParameters,
    };

    using encoding::ReadSizeTFromBytesBigEndian;

    UwbSessionData uwbSessionData;

    std::vector<encoding::TlvBer> tlvBerValues = tlvBer.GetValues();
    std::unordered_set<ParameterTag> parameterTagsRequiredNotSeen = parameterTagsRequired;

    for (const auto &tlvBerValue : tlvBerValues) {
        auto tagValue = tlvBerValue.GetTag();
        // All tags for UwbSessionData are 1-byte long, so ignore all others.
        if (std::size(tagValue) != 1) {
            continue;
        }

        // Ensure the tag has a corresponding enumeration value.
        auto parameterTag = magic_enum::enum_cast<ParameterTag>(tagValue.front());
        if (!parameterTag.has_value()) {
            continue;
        }

        // Ensure all values have non-zero payload.
        bool parameterValueWasDecoded = true;
        auto& parameterValue = tlvBerValue.GetValue();
        if (std::empty(parameterValue)) {
            continue;
        }

        switch (*parameterTag) {
        case ParameterTag::SessionDataVersion: {
            uwbSessionData.sessionDataVersion = ReadSizeTFromBytesBigEndian<decltype(uwbSessionData.sessionDataVersion)>(parameterValue);
            break;
        }
        case ParameterTag::SessionId: {
            uwbSessionData.sessionId = ReadSizeTFromBytesBigEndian<decltype(uwbSessionData.sessionId)>(parameterValue);
            break;
        }
        case ParameterTag::SubSessionId: {
            uwbSessionData.subSessionId = ReadSizeTFromBytesBigEndian<decltype(uwbSessionData.subSessionId)>(parameterValue);
            break;
        }
        case ParameterTag::ConfigurationParameters: {
            uwbSessionData.uwbConfiguration = UwbConfiguration::FromDataObject(tlvBerValue);
            break;
        }
        case ParameterTag::StaticRangingInfo: {
            uwbSessionData.staticRangingInfo = StaticRangingInfo::FromDataObject(tlvBerValue);
            break;
        }
        case ParameterTag::SecureRangingInfo: {
            uwbSessionData.secureRangingInfo = SecureRangingInfo::FromDataObject(tlvBerValue);
            break;
        }
        case ParameterTag::RegulatoryInformation: {
            uwbSessionData.regulatoryInformation = UwbRegulatoryInformation::FromDataObject(tlvBerValue);
            break;
        }
        case ParameterTag::UwbConfigAvailable: {
            uwbSessionData.uwbConfigurationAvailable = !!parameterValue.front();
            break;
        }
        default: {
            parameterValueWasDecoded = false;
            break;
        }
        }

        if (parameterValueWasDecoded) {
            parameterTagsRequiredNotSeen.erase(*parameterTag);
        }
    }

    if (!std::empty(parameterTagsRequiredNotSeen)) {
        throw UwbException(UwbStatusGeneric::SyntaxError);
    }

    return uwbSessionData;
}
