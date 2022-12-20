
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

std::optional<uint32_t>
UwbConfiguration::GetFiraPhyVersion() const noexcept
{
    return m_firaPhyVersion;
}

std::optional<uint32_t>
UwbConfiguration::GetFiraMacVersion() const noexcept
{
    return m_firaMacVersion;
}

std::optional<uwb::protocol::fira::DeviceRole>
UwbConfiguration::GetDeviceRole() const noexcept
{
    return m_deviceRole;
}

std::optional<uwb::protocol::fira::RangingConfiguration>
UwbConfiguration::GetRangingConfiguration() const noexcept
{
    return m_rangingConfiguration;
}

std::optional<uwb::protocol::fira::StsConfiguration>
UwbConfiguration::GetStsConfiguration() const noexcept
{
    return m_stsConfiguration;
}

std::optional<uwb::protocol::fira::MultiNodeMode>
UwbConfiguration::GetMultiNodeMode() const noexcept
{
    return m_multiNodeMode;
}

std::optional<uwb::protocol::fira::RangingMode>
UwbConfiguration::GetRangingTimeStruct() const noexcept
{
    return m_rangingTimeStruct;
}

std::optional<uwb::protocol::fira::SchedulingMode>
UwbConfiguration::GetSchedulingMode() const noexcept
{
    return m_schedulingMode;
}

std::optional<bool>
UwbConfiguration::GetHoppingMode() const noexcept
{
    return m_hoppingMode;
}

std::optional<bool>
UwbConfiguration::GetBlockStriding() const noexcept
{
    return m_blockStriding;
}

std::optional<uint32_t>
UwbConfiguration::GetUwbInitiationTime() const noexcept
{
    return m_uwbInitiationTime;
}

std::optional<uwb::protocol::fira::Channel>
UwbConfiguration::GetChannel() const noexcept
{
    return m_channel;
}

std::optional<uwb::protocol::fira::StsPacketConfiguration>
UwbConfiguration::GetRFrameConfig() const noexcept
{
    return m_rframeConfig;
}

std::optional<uwb::protocol::fira::ConvolutionalCodeConstraintLength>
UwbConfiguration::GetConvolutionalCodeConstraintLength() const noexcept
{
    return m_convolutionalCodeConstraintLength;
}

std::optional<uwb::protocol::fira::PrfMode>
UwbConfiguration::GetPrfMode() const noexcept
{
    return m_prfMode;
}

std::optional<uint8_t>
UwbConfiguration::GetSp0PhySetNumber() const noexcept
{
    return m_sp0PhySetNumber;
}

std::optional<uint8_t>
UwbConfiguration::GetSp1PhySetNumber() const noexcept
{
    return m_sp1PhySetNumber;
}

std::optional<uint8_t>
UwbConfiguration::GetSp3PhySetNumber() const noexcept
{
    return m_sp3PhySetNumber;
}

std::optional<uint8_t>
UwbConfiguration::GetPreableCodeIndex() const noexcept
{
    return m_preableCodeIndex;
}

std::optional<std::unordered_set<uwb::protocol::fira::ResultReportConfiguration>>
UwbConfiguration::GetResultReportConfigurations() const noexcept
{
    return m_resultReportConfigurations;
}

std::optional<uwb::UwbMacAddressType>
UwbConfiguration::GetMacAddressMode() const noexcept
{
    return m_macAddressMode;
}

std::optional<uwb::UwbMacAddress>
UwbConfiguration::GetControleeShortMacAddress() const noexcept
{
    return m_controleeShortMacAddress;
}

std::optional<uwb::UwbMacAddress>
UwbConfiguration::GetControllerMacAddress() const noexcept
{
    return m_controllerMacAddress;
}

std::optional<uint8_t>
UwbConfiguration::GetSlotsPerRangingRound() const noexcept
{
    return m_slotsPerRangingRound;
}

std::optional<uint8_t>
UwbConfiguration::GetMaxContentionPhaseLength() const noexcept
{
    return m_maxContentionPhaseLength;
}

std::optional<uint8_t>
UwbConfiguration::GetSlotDuration() const noexcept
{
    return m_slotDuration;
}

std::optional<uint16_t>
UwbConfiguration::GetRangingInterval() const noexcept
{
    return m_rangingInterval;
}

std::optional<uint8_t>
UwbConfiguration::GetKeyRotationRate() const noexcept
{
    return m_keyRotationRate;
}

std::optional<uwb::UwbMacAddressFcsType>
UwbConfiguration::GetMacAddressFcsType() const noexcept
{
    return m_macAddressFcsType;
}

std::optional<uint16_t>
UwbConfiguration::GetMaxRangingRoundRetry() const noexcept
{
    return m_maxRangingRoundRetry;
}

const std::unordered_map<uwb::protocol::fira::UwbConfiguration::ParameterTag, std::any>& 
UwbConfiguration::GetValueMap() const noexcept
{
    return m_values;
}

bool
UwbConfiguration::operator==(const UwbConfiguration& /* other */) const noexcept
{
    // TODO
    return false;
}
