
#include <functional>
#include <ranges>
#include <stdexcept>
#include <type_traits>
#include <variant>

#include <notstd/type_traits.hxx>

#include <tlv/TlvBer.hxx>
#include <tlv/TlvSerialize.hxx>
#include <uwb/protocols/fira/UwbConfiguration.hxx>
#include <uwb/protocols/fira/UwbConfigurationBuilder.hxx>

using namespace uwb::protocol::fira;

/* static */
UwbConfiguration::Builder
UwbConfiguration::Create() noexcept
{
    return UwbConfiguration::Builder();
}

/* static */
const std::unordered_set<ResultReportConfiguration> UwbConfiguration::ResultReportConfigurationsDefault = {
    ResultReportConfiguration::AoAAzimuthReport
};

std::unique_ptr<encoding::TlvBer>
UwbConfiguration::ToDataObject() const
{
    using encoding::TlvBer, encoding::GetBytesBigEndianFromBitMap;

    TlvBer::Builder builder{};

    builder.SetTag(UwbConfiguration::Tag);

    for (const auto &[parameterTag, parameterValueVariant] : m_values) {
        auto valueBuilder = TlvBer::Builder().SetTag(notstd::to_underlying(parameterTag));
        std::visit([&valueBuilder, &parameterTag](auto &&parameterValue) {
            using ParameterValueT = std::decay_t<decltype(parameterValue)>;
            if constexpr (std::is_enum_v<ParameterValueT>) {
                valueBuilder.SetValue(notstd::to_underlying(parameterValue));
            } else if constexpr (std::is_same_v<ParameterValueT, uint8_t> || std::is_same_v<ParameterValueT, std::array<uint8_t, StaticStsInitializationVectorLength>>) {
                valueBuilder.SetValue(parameterValue);
            } else if constexpr (std::is_unsigned_v<ParameterValueT>) {
                auto valueBytes = GetBytesBigEndianFromBitMap(parameterValue, sizeof parameterValue);
                valueBuilder.SetValue(valueBytes);
            } else if constexpr (std::is_same_v<ParameterValueT, ::uwb::UwbMacAddress>) {
                auto valueBytes = parameterValue.GetValue();
                valueBuilder.SetValue(valueBytes);
            } else if constexpr (std::is_same_v<ParameterValueT, std::unordered_set<::uwb::UwbMacAddress>>) {
                std::vector<uint8_t> valueBytes{};
                for (const auto &uwbMacAddress : parameterValue) {
                    auto value = parameterValue.GetValue();
                    std::ranges::copy(valueBytes, std::back_inserter(value));
                }
                valueBuilder.SetValue(valueBytes);
            } else if constexpr (std::is_same_v<ParameterValueT, std::unordered_set<ResultReportConfiguration>>) {
                uint8_t value = 0;
                for (const auto &resultReportConfiguration : parameterValue) {
                    value |= notstd::to_underlying(resultReportConfiguration);
                }
                valueBuilder.SetValue(value);
            }
        },
            parameterValueVariant);
        builder.AddTlv(valueBuilder.Build());
    }

    auto tlvBerResult = std::make_unique<TlvBer>();
    *tlvBerResult = builder.Build();

    return tlvBerResult;
}

/* static */
UwbConfiguration
UwbConfiguration::FromDataObject(const encoding::TlvBer & /* tlv */)
{
    throw std::logic_error("not implemented");
}

std::optional<uint16_t>
UwbConfiguration::GetFiraPhyVersion() const noexcept
{
    return GetValue<uint16_t>(ParameterTag::FiraPhyVersion);
}

std::optional<uint16_t>
UwbConfiguration::GetFiraMacVersion() const noexcept
{
    return GetValue<uint16_t>(ParameterTag::FiraMacVersion);
}

std::optional<DeviceRole>
UwbConfiguration::GetDeviceRole() const noexcept
{
    return GetValue<DeviceRole>(ParameterTag::DeviceRole);
}

std::optional<RangingMethod>
UwbConfiguration::GetRangingMethod() const noexcept
{
    return GetValue<RangingMethod>(ParameterTag::RangingMethod);
}

std::optional<StsConfiguration>
UwbConfiguration::GetStsConfiguration() const noexcept
{
    return GetValue<StsConfiguration>(ParameterTag::StsConfig);
}

std::optional<MultiNodeMode>
UwbConfiguration::GetMultiNodeMode() const noexcept
{
    return GetValue<MultiNodeMode>(ParameterTag::MultiNodeMode);
}

std::optional<RangingMode>
UwbConfiguration::GetRangingTimeStruct() const noexcept
{
    return GetValue<RangingMode>(ParameterTag::RangingTimeStruct);
}

std::optional<SchedulingMode>
UwbConfiguration::GetSchedulingMode() const noexcept
{
    return GetValue<SchedulingMode>(ParameterTag::ScheduledMode);
}

std::optional<bool>
UwbConfiguration::GetHoppingMode() const noexcept
{
    return GetValue<bool>(ParameterTag::HoppingMode);
}

std::optional<bool>
UwbConfiguration::GetBlockStriding() const noexcept
{
    return GetValue<bool>(ParameterTag::BlockStriding);
}

std::optional<uint32_t>
UwbConfiguration::GetUwbInitiationTime() const noexcept
{
    return GetValue<uint32_t>(ParameterTag::UwbInitiationTime);
}

std::optional<Channel>
UwbConfiguration::GetChannel() const noexcept
{
    return GetValue<Channel>(ParameterTag::ChannelNumber);
}

std::optional<StsPacketConfiguration>
UwbConfiguration::GetRFrameConfig() const noexcept
{
    return GetValue<StsPacketConfiguration>(ParameterTag::RFrameConfig);
}

std::optional<ConvolutionalCodeConstraintLength>
UwbConfiguration::GetConvolutionalCodeConstraintLength() const noexcept
{
    return GetValue<ConvolutionalCodeConstraintLength>(ParameterTag::CcConstraintLength);
}

std::optional<PrfMode>
UwbConfiguration::GetPrfMode() const noexcept
{
    return GetValue<PrfMode>(ParameterTag::PrfMode);
}

std::optional<uint8_t>
UwbConfiguration::GetSp0PhySetNumber() const noexcept
{
    return GetValue<uint8_t>(ParameterTag::Sp0PhySetNumber);
}

std::optional<uint8_t>
UwbConfiguration::GetSp1PhySetNumber() const noexcept
{
    return GetValue<uint8_t>(ParameterTag::Sp1PhySetNumber);
}

std::optional<uint8_t>
UwbConfiguration::GetSp3PhySetNumber() const noexcept
{
    return GetValue<uint8_t>(ParameterTag::Sp3PhySetNumber);
}

std::optional<uint8_t>
UwbConfiguration::GetPreambleCodeIndex() const noexcept
{
    return GetValue<uint8_t>(ParameterTag::PreambleCodeIndex);
}

std::unordered_set<ResultReportConfiguration>
UwbConfiguration::GetResultReportConfigurations() const noexcept
{
    auto it = m_values.find(ParameterTag::ResultReportConfig);
    return (it != std::cend(m_values))
        ? std::get<std::unordered_set<ResultReportConfiguration>>(it->second)
        : std::unordered_set<ResultReportConfiguration>{};
}

std::optional<uwb::UwbMacAddressType>
UwbConfiguration::GetMacAddressMode() const noexcept
{
    return GetValue<uwb::UwbMacAddressType>(ParameterTag::MacAddressMode);
}

std::optional<uwb::UwbMacAddress>
UwbConfiguration::GetControleeShortMacAddress() const noexcept
{
    return GetValue<uwb::UwbMacAddress>(ParameterTag::ControleeShortMacAddress);
}

std::optional<uwb::UwbMacAddress>
UwbConfiguration::GetControllerMacAddress() const noexcept
{
    return GetValue<uwb::UwbMacAddress>(ParameterTag::ControllerMacAddress);
}

std::optional<uint8_t>
UwbConfiguration::GetSlotsPerRangingRound() const noexcept
{
    return GetValue<uint8_t>(ParameterTag::SlotsPerRr);
}

std::optional<uint8_t>
UwbConfiguration::GetMaxContentionPhaseLength() const noexcept
{
    return GetValue<uint8_t>(ParameterTag::MaxContentionPhaseLength);
}

std::optional<uint16_t>
UwbConfiguration::GetSlotDuration() const noexcept
{
    return GetValue<uint16_t>(ParameterTag::SlotDuration);
}

std::optional<uint16_t>
UwbConfiguration::GetRangingInterval() const noexcept
{
    return GetValue<uint16_t>(ParameterTag::RangingInterval);
}

std::optional<uint8_t>
UwbConfiguration::GetKeyRotationRate() const noexcept
{
    return GetValue<uint8_t>(ParameterTag::KeyRotationRate);
}

std::optional<uwb::UwbMacAddressFcsType>
UwbConfiguration::GetMacAddressFcsType() const noexcept
{
    return GetValue<uwb::UwbMacAddressFcsType>(ParameterTag::MacFcsType);
}

std::optional<uint16_t>
UwbConfiguration::GetMaxRangingRoundRetry() const noexcept
{
    return GetValue<uint16_t>(ParameterTag::MaxRrRetry);
}

const std::unordered_map<UwbConfiguration::ParameterTag, UwbConfiguration::ParameterTypesVariant> &
UwbConfiguration::GetValueMap() const noexcept
{
    return m_values;
}
