
#include <notstd/utility.hxx>
#include <tlv/TlvSerialize.hxx>
#include <uwb/protocols/fira/UwbRegulatoryInformation.hxx>

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
                .SetValue(notstd::to_underlying(InformationSource))
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
UwbRegulatoryInformation::FromDataObject(const encoding::TlvBer& tlv)
{
    return {};
}
