
#ifndef UWB_CONFIGURATION_BUILDER_HXX
#define UWB_CONFIGURATION_BUILDER_HXX

#include <uwb/protocols/fira/FiraDevice.hxx>
#include <uwb/protocols/fira/UwbConfiguration.hxx>

namespace uwb::protocol::fira
{
/**
 * @brief Builds UwbConfiguration objects.
 */
class UwbConfiguration::Builder
{
public:
    Builder();

    /**
     * @brief Operator which returns the built UwbConfiguration object as an
     * implicit conversion. Following invocation, the state is reset and the
     * builder may be re-used. 
     * 
     * @return UwbConfiguration 
     */
    operator UwbConfiguration() noexcept;

    UwbConfiguration::Builder&
    SetFiraVersionPhy(uint16_t version) noexcept;

    UwbConfiguration::Builder&
    SetFiraVersionMac(uint16_t version) noexcept;

    UwbConfiguration::Builder&
    SetDeviceRole(uwb::protocol::fira::DeviceRole deviceRole) noexcept;

    UwbConfiguration::Builder&
    SetRangingMethod(uwb::protocol::fira::RangingMethod rangingMethod) noexcept;

    UwbConfiguration::Builder&
    SetStsConfiguration(uwb::protocol::fira::StsConfiguration stsConfiguration) noexcept;

    UwbConfiguration::Builder&
    SetMultiNodeMode(uwb::protocol::fira::MultiNodeMode multiNodeMode) noexcept;

    UwbConfiguration::Builder&
    SetRangingTimeStruct(uwb::protocol::fira::RangingMode rangingTimeStruct) noexcept;

    UwbConfiguration::Builder&
    SetSchedulingMode(uwb::protocol::fira::SchedulingMode schedulingMode) noexcept;

    UwbConfiguration::Builder&
    SetHoppingMode(bool hoppingMode) noexcept;

    UwbConfiguration::Builder&
    SetBlockStriding(bool blockStriding) noexcept;

    UwbConfiguration::Builder&
    SetUwbInitiationTime(uint32_t uwbInitiationTime) noexcept;

    UwbConfiguration::Builder&
    SetChannel(uwb::protocol::fira::Channel channel) noexcept;

    UwbConfiguration::Builder&
    SetStsPacketConfiguration(uwb::protocol::fira::StsPacketConfiguration stsPacketConfiguration) noexcept;

    UwbConfiguration::Builder&
    SetConvolutionalCodeConstraintLength(uwb::protocol::fira::ConvolutionalCodeConstraintLength convolutionalCodeConstraintLength) noexcept;

    UwbConfiguration::Builder&
    SetPrfMode(uwb::protocol::fira::PrfMode prfMode) noexcept;

    UwbConfiguration::Builder&
    SetSp0PhySetNumber(uint8_t sp0PhySetNumber) noexcept;

    UwbConfiguration::Builder&
    SetSp1PhySetNumber(uint8_t sp1PhySetNumber) noexcept;

    UwbConfiguration::Builder&
    SetSp3PhySetNumber(uint8_t sp3PhySetNumber) noexcept;

    UwbConfiguration::Builder&
    SetPreableCodeIndex(uint8_t preambleCodeIndex) noexcept;

    UwbConfiguration::Builder&
    AddResultReportConfiguration(uwb::protocol::fira::ResultReportConfiguration resultReportConfiguration) noexcept;

    UwbConfiguration::Builder&
    SetMacAddressType(uwb::UwbMacAddressType macAddressType) noexcept;

    UwbConfiguration::Builder&
    SetMacAddressFcsType(uwb::UwbMacAddressFcsType fcsType) noexcept;

    UwbConfiguration::Builder&
    SetMacAddressControleeShort(uwb::UwbMacAddress macAddress) noexcept;

    UwbConfiguration::Builder&
    SetMacAddressController(uwb::UwbMacAddress macAddress) noexcept;

    UwbConfiguration::Builder&
    SetMaxSlotsPerRangingRound(uint8_t slotsPerRangingRound) noexcept;

    UwbConfiguration::Builder&
    SetMaxContentionPhaseLength(uint8_t maxContentionPhaseLength) noexcept;

    UwbConfiguration::Builder&
    SetSlotDuration(uint16_t maxContentionPhaseLength) noexcept;

    UwbConfiguration::Builder&
    SetRangingInterval(uint16_t rangingInterval) noexcept;

    UwbConfiguration::Builder&
    SetKeyRotationRate(uint8_t keyRotationRate) noexcept;

    UwbConfiguration::Builder&
    SetMaxRangingRoundRetry(uint16_t maxRangingRoundRetry) noexcept;

    /**
     * @brief No-op helper to supply more semantic information while chaining arguments.
     * 
     * @return UwbConfiguration::Builder& 
     */
    UwbConfiguration::Builder&
    With() noexcept;

    /**
     * @brief No-op helper to group fira version arguments.
     * 
     * @return UwbConfiguration::Builder& 
     */
    UwbConfiguration::Builder&
    FiraVersion() noexcept;

    UwbConfiguration::Builder&
    Phy(uint16_t version) noexcept;

    UwbConfiguration::Builder&
    Mac(uint16_t version) noexcept;

    UwbConfiguration::Builder&
    DeviceRole(uwb::protocol::fira::DeviceRole deviceRole) noexcept;

    UwbConfiguration::Builder&
    RangingMethod(uwb::protocol::fira::RangingMethod rangingMethod) noexcept;

    UwbConfiguration::Builder&
    StsConfiguration(uwb::protocol::fira::StsConfiguration stsConfiguration) noexcept;

    UwbConfiguration::Builder&
    MultiNodeMode(uwb::protocol::fira::MultiNodeMode multiNodeMode) noexcept;

    UwbConfiguration::Builder&
    RangingTimeStruct(uwb::protocol::fira::RangingMode rangingTimeStruct) noexcept;

    UwbConfiguration::Builder&
    SchedulingMode(uwb::protocol::fira::SchedulingMode schedulingMode) noexcept;

    UwbConfiguration::Builder&
    UwbInitiationTime(uint32_t uwbInitiationTime) noexcept;

    UwbConfiguration::Builder&
    OnChannel(uwb::protocol::fira::Channel channel) noexcept;

    UwbConfiguration::Builder&
    StsPacketConfiguration(uwb::protocol::fira::StsPacketConfiguration stsPacketConfiguration) noexcept;

    UwbConfiguration::Builder&
    ConvolutionalCodeConstraintLength(uwb::protocol::fira::ConvolutionalCodeConstraintLength convolutionalCodeConstraintLength) noexcept;

    UwbConfiguration::Builder&
    PrfMode(uwb::protocol::fira::PrfMode prfMode) noexcept;

    /**
     * @brief No-op helper to group PHY set number arguments.
     * 
     * @return UwbConfiguration::Builder& 
     */
    UwbConfiguration::Builder&
    PhySetNumber() noexcept;

    UwbConfiguration::Builder&
    Sp0(uint8_t sp0PhySetNumber) noexcept;

    UwbConfiguration::Builder&
    Sp1(uint8_t sp1PhySetNumber) noexcept;

    UwbConfiguration::Builder&
    Sp3(uint8_t sp3PhySetNumber) noexcept;

    UwbConfiguration::Builder&
    PreableCodeIndex(uint8_t preambleCodeIndex) noexcept;

    /**
     * @brief No-op helper to supply more semantic information while chaining arguments.
     * 
     * @return UwbConfiguration::Builder& 
     */
    UwbConfiguration::Builder&
    Supports() noexcept;

    UwbConfiguration::Builder&
    ResultReportConfiguration(uwb::protocol::fira::ResultReportConfiguration resultReportConfiguration) noexcept;

    /**
     * @brief No-op helper to group mac address arguments.
     * 
     * @return UwbConfiguration::Builder& 
     */
    UwbConfiguration::Builder&
    MacAddress() noexcept;

    UwbConfiguration::Builder&
    Type(uwb::UwbMacAddressType macAddressType) noexcept;

    UwbConfiguration::Builder&
    OfControleeShort(uwb::UwbMacAddress macAddress) noexcept;

    UwbConfiguration::Builder&
    OfController(uwb::UwbMacAddress macAddress) noexcept;

    UwbConfiguration::Builder&
    FcsType(uwb::UwbMacAddressFcsType fcsType) noexcept;

    UwbConfiguration::Builder&
    SlotsPerRangingRound(uint8_t slotsPerRangingRound) noexcept;

    /**
     * @brief No-op helper to group maximum/limit arguments.
     * 
     * @return UwbConfiguration::Builder& 
     */
    UwbConfiguration::Builder&
    Maximum() noexcept;

    UwbConfiguration::Builder&
    ContentionPhaseLength(uint8_t maxContentionPhaseLength) noexcept;

    UwbConfiguration::Builder&
    SlotDuration(uint16_t maxContentionPhaseLength) noexcept;

    UwbConfiguration::Builder&
    RangingInterval(uint16_t rangingInterval) noexcept;

    UwbConfiguration::Builder&
    KeyRotationRate(uint8_t keyRotationRate) noexcept;

    UwbConfiguration::Builder&
    RangingRoundRetry(uint16_t maxRangingRoundRetry) noexcept;

private:
    UwbConfiguration m_uwbConfiguration;
    std::unordered_map<uwb::protocol::fira::UwbConfiguration::ParameterTag, uwb::protocol::fira::UwbConfiguration::ParameterTypesVariant>& m_values;
};

} // namespace uwb::protocol::fira

#endif // UWB_CONFIGURATION_BUILDER_HXX
