
#include <unordered_set>

#include <magic_enum.hpp>
#include <notstd/utility.hxx>
#include <tlv/TlvSerialize.hxx>
#include <uwb/protocols/fira/UwbRegulatoryInformation.hxx>
#include <uwb/protocols/fira/UwbException.hxx>

using namespace uwb::protocol::fira;

std::unique_ptr<encoding::TlvBer>
UwbRegulatoryInformation::ToDataObject() const
{
    using encoding::TlvBer, encoding::GetBytesBigEndianFromBitMap;

    auto tlvBerResult = std::make_unique<TlvBer>();
    *tlvBerResult = TlvBer::Builder()
        .SetTag(Tag)
        // INFORMATION_SOURCE
        .AddTlv(
            TlvBer::Builder()
                .SetTag(notstd::to_underlying(ParameterTag::InformationSource))
                .SetValue(notstd::to_underlying(Source))
                .Build())
        // OUTDOOR_PERMITTED
        .AddTlv(
            TlvBer::Builder()
                .SetTag(notstd::to_underlying(ParameterTag::OutdoorPermitted))
                .SetValue(!!OutdoorPermitted)
                .Build())
        // COUNTRY_CODE
        .AddTlv(
            TlvBer::Builder()
                .SetTag(notstd::to_underlying(ParameterTag::CountryCode))
                .SetValue(GetBytesBigEndianFromBitMap(CountryCode, sizeof CountryCode))
                .Build())
        // TIMESTAMP
        .AddTlv(
            TlvBer::Builder()
                .SetTag(notstd::to_underlying(ParameterTag::Timestamp))
                .SetValue(GetBytesBigEndianFromBitMap(Timestamp, sizeof Timestamp))
                .Build())
        // CHANNEL5
        .AddTlv(
            TlvBer::Builder()
                .SetTag(notstd::to_underlying(ParameterTag::Channel5))
                .SetValue(MaximumTransmissionPower.at(Channel::C5))
                .Build())
        // CHANNEL6
        .AddTlv(
            TlvBer::Builder()
                .SetTag(notstd::to_underlying(ParameterTag::Channel6))
                .SetValue(MaximumTransmissionPower.at(Channel::C6))
                .Build())
        // CHANNEL8
        .AddTlv(
            TlvBer::Builder()
                .SetTag(notstd::to_underlying(ParameterTag::Channel8))
                .SetValue(MaximumTransmissionPower.at(Channel::C8))
                .Build())
        // CHANNEL9
        .AddTlv(
            TlvBer::Builder()
                .SetTag(notstd::to_underlying(ParameterTag::Channel9))
                .SetValue(MaximumTransmissionPower.at(Channel::C9))
                .Build())
        // CHANNEL10
        .AddTlv(
            TlvBer::Builder()
                .SetTag(notstd::to_underlying(ParameterTag::Channel10))
                .SetValue(MaximumTransmissionPower.at(Channel::C10))
                .Build())
        // CHANNEL12
        .AddTlv(
            TlvBer::Builder()
                .SetTag(notstd::to_underlying(ParameterTag::Channel12))
                .SetValue(MaximumTransmissionPower.at(Channel::C12))
                .Build())
        // CHANNEL13
        .AddTlv(
            TlvBer::Builder()
                .SetTag(notstd::to_underlying(ParameterTag::Channel13))
                .SetValue(MaximumTransmissionPower.at(Channel::C13))
                .Build())
        // CHANNEL14
        .AddTlv(
            TlvBer::Builder()
                .SetTag(notstd::to_underlying(ParameterTag::Channel14))
                .SetValue(MaximumTransmissionPower.at(Channel::C14))
                .Build())
        .Build();

    return tlvBerResult;
}

/* static */
UwbRegulatoryInformation
UwbRegulatoryInformation::FromDataObject(const encoding::TlvBer& tlvBer)
{
     using encoding::ReadSizeTFromBytesBigEndian;

    UwbRegulatoryInformation uwbRegulatoryInformation{};
    std::unordered_set<ParameterTag> parameterTagsDecoded{};
    std::vector<encoding::TlvBer> tlvBerValues = tlvBer.GetValues();

    for (const auto &tlvBerValue : tlvBerValues) {
        auto tagValue = tlvBerValue.GetTag();
        // All tags for UwbRegulatoryInformation are 1-byte long, so ignore all others.
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
        case ParameterTag::InformationSource: {
            auto valueEnum = magic_enum::enum_cast<InformationSource>(parameterValue.front());
            if (valueEnum.has_value()) {
                uwbRegulatoryInformation.Source = *valueEnum;    
            } else {
                parameterValueWasDecoded = false;
            }
            break;
        }
        case ParameterTag::OutdoorPermitted: {
            uwbRegulatoryInformation.OutdoorPermitted = !!parameterValue.front();
            break;
        }
        case ParameterTag::CountryCode: {
            uwbRegulatoryInformation.CountryCode = ReadSizeTFromBytesBigEndian<decltype(uwbRegulatoryInformation.CountryCode)>(parameterValue);
            break;
        }
        case ParameterTag::Timestamp: {
            uwbRegulatoryInformation.Timestamp = ReadSizeTFromBytesBigEndian<decltype(uwbRegulatoryInformation.Timestamp)>(parameterValue);
            break;
        }
        case ParameterTag::Channel5: {
            uwbRegulatoryInformation.MaximumTransmissionPower[Channel::C5] = parameterValue.front();
            break;
        }
        case ParameterTag::Channel6: {
            uwbRegulatoryInformation.MaximumTransmissionPower[Channel::C6] = parameterValue.front();
            break;
        }
         case ParameterTag::Channel8: {
            uwbRegulatoryInformation.MaximumTransmissionPower[Channel::C8] = parameterValue.front();
            break;
        }
        case ParameterTag::Channel9: {
            uwbRegulatoryInformation.MaximumTransmissionPower[Channel::C9] = parameterValue.front();
            break;
        }
        case ParameterTag::Channel10: {
            uwbRegulatoryInformation.MaximumTransmissionPower[Channel::C10] = parameterValue.front();
            break;
        }
        case ParameterTag::Channel12: {
            uwbRegulatoryInformation.MaximumTransmissionPower[Channel::C12] = parameterValue.front();
            break;
        }
        case ParameterTag::Channel13: {
            uwbRegulatoryInformation.MaximumTransmissionPower[Channel::C13] = parameterValue.front();
            break;
        }
        case ParameterTag::Channel14: {
            uwbRegulatoryInformation.MaximumTransmissionPower[Channel::C14] = parameterValue.front();
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

    return uwbRegulatoryInformation;
}
