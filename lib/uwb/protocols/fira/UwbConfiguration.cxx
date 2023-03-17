
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
    return GetOobValue<uint16_t>(ParameterTag::FiraPhyVersion);
}

std::optional<uint16_t>
UwbConfiguration::GetFiraMacVersion() const noexcept
{
    return GetOobValue<uint16_t>(ParameterTag::FiraMacVersion);
}

std::optional<DeviceRole>
UwbConfiguration::GetDeviceRole() const noexcept
{
    return GetUciValue<DeviceRole>(UwbApplicationConfigurationParameterType::DeviceRole);
}

std::optional<RangingMethod>
UwbConfiguration::GetRangingMethod() const noexcept
{
    return GetOobValue<RangingMethod>(ParameterTag::RangingMethod);
}

std::optional<StsConfiguration>
UwbConfiguration::GetStsConfiguration() const noexcept
{
    return GetOobValue<StsConfiguration>(ParameterTag::StsConfig);
}

std::optional<MultiNodeMode>
UwbConfiguration::GetMultiNodeMode() const noexcept
{
    return GetUciValue<MultiNodeMode>(UwbApplicationConfigurationParameterType::MultiNodeMode);
}

std::optional<RangingMode>
UwbConfiguration::GetRangingTimeStruct() const noexcept
{
    return GetUciValue<RangingMode>(UwbApplicationConfigurationParameterType::RangingTimeStruct);
}

std::optional<SchedulingMode>
UwbConfiguration::GetSchedulingMode() const noexcept
{
    return GetUciValue<SchedulingMode>(UwbApplicationConfigurationParameterType::ScheduledMode);
}

std::optional<bool>
UwbConfiguration::GetHoppingMode() const noexcept
{
    return GetUciValue<bool>(UwbApplicationConfigurationParameterType::HoppingMode);
}

std::optional<bool>
UwbConfiguration::GetBlockStriding() const noexcept
{
    return GetOobValue<bool>(ParameterTag::BlockStriding);
}

std::optional<uint8_t>
UwbConfiguration::GetBlockStrideLength() const noexcept
{
    return GetUciValue<uint8_t>(UwbApplicationConfigurationParameterType::BlockStrideLength);
}

std::optional<uint32_t>
UwbConfiguration::GetUwbInitiationTime() const noexcept
{
    return GetUciValue<uint32_t>(UwbApplicationConfigurationParameterType::UwbInitiationTime);
}

std::optional<Channel>
UwbConfiguration::GetChannel() const noexcept
{
    return GetUciValue<Channel>(UwbApplicationConfigurationParameterType::ChannelNumber);
}

std::optional<StsPacketConfiguration>
UwbConfiguration::GetRFrameConfig() const noexcept
{
    return GetUciValue<StsPacketConfiguration>(UwbApplicationConfigurationParameterType::RFrameConfiguration);
}

std::optional<ConvolutionalCodeConstraintLength>
UwbConfiguration::GetConvolutionalCodeConstraintLength() const noexcept
{
    return GetOobValue<ConvolutionalCodeConstraintLength>(ParameterTag::CcConstraintLength);
}

std::optional<PrfMode>
UwbConfiguration::GetPrfMode() const noexcept
{
    return GetUciValue<PrfMode>(UwbApplicationConfigurationParameterType::PrfMode);
}

std::optional<uint8_t>
UwbConfiguration::GetSp0PhySetNumber() const noexcept
{
    return GetOobValue<uint8_t>(ParameterTag::Sp0PhySetNumber);
}

std::optional<uint8_t>
UwbConfiguration::GetSp1PhySetNumber() const noexcept
{
    return GetOobValue<uint8_t>(ParameterTag::Sp1PhySetNumber);
}

std::optional<uint8_t>
UwbConfiguration::GetSp3PhySetNumber() const noexcept
{
    return GetOobValue<uint8_t>(ParameterTag::Sp3PhySetNumber);
}

std::optional<uint8_t>
UwbConfiguration::GetPreambleCodeIndex() const noexcept
{
    return GetUciValue<uint8_t>(UwbApplicationConfigurationParameterType::PreambleCodeIndex);
}

std::unordered_set<ResultReportConfiguration>
UwbConfiguration::GetResultReportConfigurations() const noexcept
{
    auto it = m_valuesUci.find(UwbApplicationConfigurationParameterType::ResultReportConfig);
    return (it != std::cend(m_valuesUci))
        ? std::get<std::unordered_set<ResultReportConfiguration>>(it->second)
        : std::unordered_set<ResultReportConfiguration>{};
}

std::optional<uwb::UwbMacAddressType>
UwbConfiguration::GetMacAddressMode() const noexcept
{
    return GetUciValue<::uwb::UwbMacAddressType>(UwbApplicationConfigurationParameterType::MacAddressMode);
}

std::optional<::uwb::UwbMacAddress>
UwbConfiguration::GetDeviceMacAddress() const noexcept
{
    return GetUciValue<::uwb::UwbMacAddress>(UwbApplicationConfigurationParameterType::DeviceMacAddress);
}

std::unordered_set<::uwb::UwbMacAddress>
UwbConfiguration::GetDestinationMacAddresses() const noexcept
{
    auto it = m_valuesUci.find(UwbApplicationConfigurationParameterType::ControleeMacAddress);
    return (it != std::cend(m_valuesUci))
        ? std::get<std::unordered_set<::uwb::UwbMacAddress>>(it->second)
        : std::unordered_set<::uwb::UwbMacAddress>{};
}

std::optional<uwb::UwbMacAddress>
UwbConfiguration::GetControleeShortMacAddress() const noexcept
{
    return GetOobValue<::uwb::UwbMacAddress>(ParameterTag::ControleeShortMacAddress);
}

std::optional<uwb::UwbMacAddress>
UwbConfiguration::GetControllerMacAddress() const noexcept
{
    return GetOobValue<uwb::UwbMacAddress>(ParameterTag::ControllerMacAddress);
}

std::optional<uint8_t>
UwbConfiguration::GetSlotsPerRangingRound() const noexcept
{
    return GetUciValue<uint8_t>(UwbApplicationConfigurationParameterType::SlotsPerRangingRound);
}

std::optional<uint8_t>
UwbConfiguration::GetMaxContentionPhaseLength() const noexcept
{
    return GetOobValue<uint8_t>(ParameterTag::MaxContentionPhaseLength);
}

std::optional<uint16_t>
UwbConfiguration::GetSlotDuration() const noexcept
{
    return GetUciValue<uint16_t>(UwbApplicationConfigurationParameterType::SlotDuration);
}

std::optional<uint16_t>
UwbConfiguration::GetRangingInterval() const noexcept
{
    return GetUciValue<uint16_t>(UwbApplicationConfigurationParameterType::RangingInterval);
}

std::optional<uint8_t>
UwbConfiguration::GetKeyRotationRate() const noexcept
{
    return GetUciValue<uint8_t>(UwbApplicationConfigurationParameterType::KeyRotationRate);
}

std::optional<uwb::UwbMacAddressFcsType>
UwbConfiguration::GetMacAddressFcsType() const noexcept
{
    return GetUciValue<::uwb::UwbMacAddressFcsType>(UwbApplicationConfigurationParameterType::MacFcsType);
}

std::optional<uint16_t>
UwbConfiguration::GetMaxRangingRoundRetry() const noexcept
{
    return GetUciValue<uint16_t>(UwbApplicationConfigurationParameterType::MaxRangingRoundRetry);
}

const std::unordered_map<UwbConfiguration::ParameterTag, UwbConfiguration::ValueVariantOob>&
UwbConfiguration::GetValueMapOob() const noexcept
{
    return m_valuesOob;
}

const std::unordered_map<UwbApplicationConfigurationParameterType, UwbConfiguration::ValueVariantUci>&
UwbConfiguration::GetValueMapUci() const noexcept
{
    return m_valuesUci;
}
