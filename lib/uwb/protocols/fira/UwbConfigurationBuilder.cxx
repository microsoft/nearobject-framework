
#include <uwb/protocols/fira/UwbConfigurationBuilder.hxx>

using namespace uwb::protocol::fira;

UwbConfiguration::Builder::Builder() :
    m_valuesOob(m_uwbConfiguration.m_valuesOob),
    m_valuesUci(m_uwbConfiguration.m_valuesUci)
{}

UwbConfiguration::Builder::operator UwbConfiguration() noexcept
{
    auto uwbConfiguration = std::move(m_uwbConfiguration);
    m_uwbConfiguration = {};
    return uwbConfiguration;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::With() noexcept
{
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::SetDeviceRole(uwb::protocol::fira::DeviceRole deviceRole) noexcept
{
    m_valuesUci[UwbApplicationConfigurationParameterType::DeviceRole] = deviceRole;
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::DeviceRole(uwb::protocol::fira::DeviceRole deviceRole) noexcept
{
    return SetDeviceRole(deviceRole);
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::FiraVersion() noexcept
{
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::SetFiraVersionPhy(uint16_t firaPhyVersion) noexcept
{
    m_valuesOob[ParameterTag::FiraPhyVersion] = firaPhyVersion;
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::Phy(uint16_t firaPhyVersion) noexcept
{
    return SetFiraVersionPhy(firaPhyVersion);
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::SetFiraVersionMac(uint16_t firaMacVersion) noexcept
{
    m_valuesOob[ParameterTag::FiraMacVersion] = firaMacVersion;
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::Mac(uint16_t firaMacVersion) noexcept
{
    return SetFiraVersionMac(firaMacVersion);
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::SetRangingMethod(uwb::protocol::fira::RangingMethod rangingMethod) noexcept
{
    m_valuesOob[ParameterTag::RangingMethod] = rangingMethod;
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::RangingMethod(uwb::protocol::fira::RangingMethod rangingMethod) noexcept
{
    return SetRangingMethod(rangingMethod);
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::SetStsConfiguration(uwb::protocol::fira::StsConfiguration stsConfiguration) noexcept
{
    m_valuesOob[ParameterTag::StsConfig] = stsConfiguration;
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::StsConfiguration(uwb::protocol::fira::StsConfiguration stsConfiguration) noexcept
{
    return SetStsConfiguration(stsConfiguration);
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::SetMultiNodeMode(uwb::protocol::fira::MultiNodeMode multiNodeMode) noexcept
{
    m_valuesUci[UwbApplicationConfigurationParameterType::MultiNodeMode] = multiNodeMode;
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::MultiNodeMode(uwb::protocol::fira::MultiNodeMode multiNodeMode) noexcept
{
    return SetMultiNodeMode(multiNodeMode);
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::SetRangingTimeStruct(uwb::protocol::fira::RangingMode rangingTimeStruct) noexcept
{
    m_valuesUci[UwbApplicationConfigurationParameterType::RangingTimeStruct] = rangingTimeStruct;
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::RangingTimeStruct(uwb::protocol::fira::RangingMode rangingTimeStruct) noexcept
{
    return SetRangingTimeStruct(rangingTimeStruct);
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::SetSchedulingMode(uwb::protocol::fira::SchedulingMode schedulingMode) noexcept
{
    m_valuesUci[UwbApplicationConfigurationParameterType::ScheduledMode] = schedulingMode;
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::SchedulingMode(uwb::protocol::fira::SchedulingMode schedulingMode) noexcept
{
    return SetSchedulingMode(schedulingMode);
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::SetHoppingMode(bool hoppingMode) noexcept
{
    m_valuesOob[ParameterTag::HoppingMode] = hoppingMode;
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::SetBlockStriding(bool blockStriding) noexcept
{
    m_valuesOob[ParameterTag::BlockStriding] = blockStriding;
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::SetUwbInitiationTime(uint32_t uwbInitiationTime) noexcept
{
    m_valuesOob[ParameterTag::UwbInitiationTime] = uwbInitiationTime;
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::UwbInitiationTime(uint32_t uwbInitiationTime) noexcept
{
    return SetUwbInitiationTime(uwbInitiationTime);
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::SetChannel(uwb::protocol::fira::Channel channel) noexcept
{
    m_valuesUci[UwbApplicationConfigurationParameterType::ChannelNumber] = channel;
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::OnChannel(uwb::protocol::fira::Channel channel) noexcept
{
    return SetChannel(channel);
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::SetStsPacketConfiguration(uwb::protocol::fira::StsPacketConfiguration rframeConfiguration) noexcept
{
    m_valuesUci[UwbApplicationConfigurationParameterType::RFrameConfiguration] = rframeConfiguration;
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::StsPacketConfiguration(uwb::protocol::fira::StsPacketConfiguration rframeConfiguration) noexcept
{
    return SetStsPacketConfiguration(rframeConfiguration);
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::SetConvolutionalCodeConstraintLength(uwb::protocol::fira::ConvolutionalCodeConstraintLength convolutionalCodeConstraintLength) noexcept
{
    m_valuesOob[ParameterTag::CcConstraintLength] = convolutionalCodeConstraintLength;
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::ConvolutionalCodeConstraintLength(uwb::protocol::fira::ConvolutionalCodeConstraintLength convolutionalCodeConstraintLength) noexcept
{
    return SetConvolutionalCodeConstraintLength(convolutionalCodeConstraintLength);
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::SetPrfMode(uwb::protocol::fira::PrfMode prfMode) noexcept
{
    m_valuesUci[UwbApplicationConfigurationParameterType::PrfMode] = prfMode;
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::PrfMode(uwb::protocol::fira::PrfMode prfMode) noexcept
{
    return SetPrfMode(prfMode);
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::PhySetNumber() noexcept
{
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::SetSp0PhySetNumber(uint8_t sp0PhySetNumber) noexcept
{
    m_valuesOob[ParameterTag::Sp0PhySetNumber] = sp0PhySetNumber;
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::Sp0(uint8_t sp0) noexcept
{
    return SetSp0PhySetNumber(sp0);
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::SetSp1PhySetNumber(uint8_t sp1PhySetNumber) noexcept
{
    m_valuesOob[ParameterTag::Sp1PhySetNumber] = sp1PhySetNumber;
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::Sp1(uint8_t sp1PhySetNumber) noexcept
{
    return SetSp1PhySetNumber(sp1PhySetNumber);
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::SetSp3PhySetNumber(uint8_t sp3PhySetNumber) noexcept
{
    m_valuesOob[ParameterTag::Sp3PhySetNumber] = sp3PhySetNumber;
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::Sp3(uint8_t sp3PhySetNumber) noexcept
{
    return SetSp3PhySetNumber(sp3PhySetNumber);
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::SetPreableCodeIndex(uint8_t preambleCodeIndex) noexcept
{
    m_valuesOob[ParameterTag::PreableCodeIndex] = preambleCodeIndex;
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::PreableCodeIndex(uint8_t preambleCodeIndex) noexcept
{
    return SetPreableCodeIndex(preambleCodeIndex);
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::Supports() noexcept
{
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::AddResultReportConfiguration(::uwb::protocol::fira::ResultReportConfiguration resultReportConfiguration) noexcept
{
    auto [it, _] = m_valuesUci.insert({ UwbApplicationConfigurationParameterType::ResultReportConfig, std::unordered_set<::uwb::protocol::fira::ResultReportConfiguration>{} });
    auto& resultReportConfigurations = std::get<std::unordered_set<::uwb::protocol::fira::ResultReportConfiguration>>(it->second);
    resultReportConfigurations.insert(std::move(resultReportConfiguration));

    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::ResultReportConfiguration(uwb::protocol::fira::ResultReportConfiguration resultReportConfiguration) noexcept
{
    return AddResultReportConfiguration(resultReportConfiguration);
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::MacAddress() noexcept
{
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::SetMacAddressType(uwb::UwbMacAddressType macAddressType) noexcept
{
    m_valuesUci[UwbApplicationConfigurationParameterType::MacAddressMode] = macAddressType;
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::Type(uwb::UwbMacAddressType macAddressType) noexcept
{
    return SetMacAddressType(macAddressType);
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::SetMacAddressControleeShort(uwb::UwbMacAddress macAddress) noexcept
{
    if (macAddress.GetType() == UwbMacAddressType::Short) {
        m_valuesOob[ParameterTag::ControleeShortMacAddress] = macAddress;
    }
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::OfControleeShort(uwb::UwbMacAddress macAddress) noexcept
{
    return SetMacAddressControleeShort(macAddress);
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::SetMacAddressController(uwb::UwbMacAddress macAddress) noexcept
{
    m_valuesOob[ParameterTag::ControllerMacAddress] = macAddress;
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::OfController(uwb::UwbMacAddress macAddress) noexcept
{
    return SetMacAddressController(macAddress);
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::SetMacAddressFcsType(uwb::UwbMacAddressFcsType fcsType) noexcept
{
    m_valuesUci[UwbApplicationConfigurationParameterType::MacFcsType] = fcsType;
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::FcsType(uwb::UwbMacAddressFcsType fcsType) noexcept
{
    return SetMacAddressFcsType(fcsType);
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::SetMaxSlotsPerRangingRound(uint8_t slotsPerRangingRound) noexcept
{
    m_valuesOob[ParameterTag::SlotsPerRr] = slotsPerRangingRound;
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::SlotsPerRangingRound(uint8_t slotsPerRangingRound) noexcept
{
    return SetMaxSlotsPerRangingRound(slotsPerRangingRound);
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::Maximum() noexcept
{
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::SetMaxContentionPhaseLength(uint8_t maxContentionPhaseLength) noexcept
{
    m_valuesOob[ParameterTag::MaxContentionPhaseLength] = maxContentionPhaseLength;
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::ContentionPhaseLength(uint8_t maxContentionPhaseLength) noexcept
{
    return SetMaxContentionPhaseLength(maxContentionPhaseLength);
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::SetSlotDuration(uint16_t slotDuration) noexcept
{
    m_valuesOob[ParameterTag::SlotDuration] = slotDuration;
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::SlotDuration(uint16_t slotDuration) noexcept
{
    return SetSlotDuration(slotDuration);
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::SetRangingInterval(uint16_t rangingInterval) noexcept
{
    m_valuesOob[ParameterTag::RangingInterval] = rangingInterval;
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::RangingInterval(uint16_t rangingInterval) noexcept
{
    return SetRangingInterval(rangingInterval);
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::SetKeyRotationRate(uint8_t keyRotationRate) noexcept
{
    m_valuesOob[ParameterTag::KeyRotationRate] = keyRotationRate;
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::KeyRotationRate(uint8_t keyRotationRate) noexcept
{
    return SetKeyRotationRate(keyRotationRate);
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::SetMaxRangingRoundRetry(uint16_t maxRangingRoundRetry) noexcept
{
    m_valuesOob[ParameterTag::MaxRrRetry] = maxRangingRoundRetry;
    return *this;
}

UwbConfiguration::Builder&
UwbConfiguration::Builder::RangingRoundRetry(uint16_t maxRangingRoundRetry) noexcept
{
    return SetMaxRangingRoundRetry(maxRangingRoundRetry);
}
