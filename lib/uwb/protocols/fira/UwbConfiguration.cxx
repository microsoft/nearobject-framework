
#include <stdexcept>

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

/**
 * @brief map of uci tag to a function of how to generate that uci tag given UwbConfiguration
 *
 */
const std::unordered_map<uwb::protocol::fira::UwbApplicationConfigurationParameterType, std::function<std::optional<uwb::protocol::fira::UwbApplicationConfigurationParameterValue>(UwbConfiguration*)>> uciGenerators;

std::vector<UwbApplicationConfigurationParameter>
UwbConfiguration::GetUciConfigParams()
{
    std::vector<UwbApplicationConfigurationParameter> result;
    for (const auto& [uciTag, generator] : uciGenerators) {
        const auto uciValue = generator(this);
        if (not uciValue) {
            continue;
        }
        result.push_back(UwbApplicationConfigurationParameter{
            .Type = uciTag,
            .Value = uciValue.value() });
    }
    return result;
}

std::unique_ptr<encoding::TlvBer>
UwbConfiguration::ToDataObject() const
{
    throw std::logic_error("not implemented");
}

/* static */
UwbConfiguration
UwbConfiguration::FromDataObject(const encoding::TlvBer& /* tlv */)
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

const std::unordered_map<UwbConfiguration::ParameterTag, UwbConfiguration::ParameterTypesVariant>&
UwbConfiguration::GetValueMap() const noexcept
{
    return m_values;
}
