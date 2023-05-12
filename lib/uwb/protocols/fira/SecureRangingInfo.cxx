
#include <unordered_set>

#include <magic_enum.hpp>
#include <notstd/utility.hxx>
#include <tlv/TlvSerialize.hxx>
#include <uwb/protocols/fira/SecureRangingInfo.hxx>
#include <uwb/protocols/fira/UwbException.hxx>

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
SecureRangingInfo::FromDataObject(const encoding::TlvBer& tlvBer)
{
    using encoding::ReadSizeTFromBytesBigEndian;

    SecureRangingInfo secureRangingInfo{};
    std::unordered_set<ParameterTag> parameterTagsDecoded{};
    std::vector<encoding::TlvBer> tlvBerValues = tlvBer.GetValues();

    for (const auto &tlvBerValue : tlvBerValues) {
        auto tagValue = tlvBerValue.GetTag();
        // All tags for SecureRangingInfo are 1-byte long, so ignore all others.
        if (std::size(tagValue) != 1) {
            continue;
        }

        // Ensure the tag has a corresponding enumeration value.
        auto parameterTag = magic_enum::enum_cast<ParameterTag>(tagValue.front());
        if (!parameterTag.has_value()) {
            continue;
        }

        bool parameterValueWasDecoded = true;
        auto& parameterValue = tlvBerValue.GetValue();

        switch (*parameterTag) {
        case ParameterTag::UwbSessionKeyInfo: {
            secureRangingInfo.UwbSessionKeyInfo = parameterValue;
            break;
        }
        case ParameterTag::ResponderSpecificSubSessionKeyInfo: {
            secureRangingInfo.ResponderSpecificSubSessionKeyInfo = parameterValue;
            break;
        }
        case ParameterTag::SusAdditionalParameters: {
            secureRangingInfo.SusAdditionalParameters = parameterValue;
            break;
        }
        default: {
            parameterValueWasDecoded = false;
            break;
        }
        }

        if (parameterValueWasDecoded) {
            parameterTagsDecoded.insert(*parameterTag);
        }
    }

    if (std::size(parameterTagsDecoded) < magic_enum::enum_count<ParameterTag>()) {
        throw UwbException(UwbStatusGeneric::SyntaxError);
    }

    return secureRangingInfo;
}
