
#ifndef UWB_CONFIGURATION_HXX
#define UWB_CONFIGURATION_HXX

#include <compare>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <unordered_set>

#include <TlvBer.hxx>
#include <notstd/hash.hxx>

#include <uwb/UwbMacAddress.hxx>
#include <uwb/protocols/fira/FiraDevice.hxx>
#include <uwb/protocols/fira/RangingConfiguration.hxx>

namespace uwb::protocol::fira
{
/**
 * @brief Describes UWB configuration parameters for a session.
 *
 * See FiRa Consortium Common Service Management Layer Technical Specification
 * v1.0.0, Section 6.4.3, 'UWB_CONFIGURATION', pages 50-54.
 */
struct UwbConfiguration
{
    class Builder;
    friend class Builder;

    /**
     * @brief See FiRa Consortium Common Service Management Layer Technical
     * Specification v1.0.0, Section 7.5.3.2, 'UWB Session Data structure',
     * Table 53, pages 103-107.
     */
    static constexpr uint8_t Tag = 0xA3;

    /**
     * @brief See FiRa Consortium Common Service Management Layer Technical
     * Specification v1.0.0, Section 7.5.3.2, 'UWB Session Data structure',
     * Table 53, pages 103-107.
     */
    enum class ParameterTag : uint8_t {
        FiraPhyVersion = 0x80,
        FiraMacVersion = 0x81,
        DeviceRole = 0x82,
        RangingMethod = 0x83,
        StsConfig = 0x84,
        MultiNodeMode = 0x85,
        RangingTimeStruct = 0x86,
        ScheduledMode = 0x87,
        HoppingMode = 0x88,
        BlockStriding = 0x89,
        UwbInitiationTime = 0x8A,
        ChannelNumber = 0x8B,
        RFrameConfig = 0x8C,
        CcConstraintLength = 0x8D,
        PrfMode = 0x8E,
        Sp0PhySetNumber = 0x8F,
        Sp1PhySetNumber = 0x90,
        Sp3PhySetNumber = 0x91,
        PreableCodeIndex = 0x92,
        ResultReportConfig = 0x93,
        MacAddressMode = 0x94,
        ControleeShortMacAddress = 0x95,
        ControllerMacAddress = 0x96,
        SlotsPerRr = 0x97,
        MaxContentionPhaseLength = 0x98,
        SlotDuration = 0x99,
        RangingInternval = 0x9A,
        KeyRotationRate = 0x9B,
        MacFcsType = 0x9C,
        MaxRrRetry = 0x9D,
    };

    /**
     * @brief Default values, if omitted, per FiRa.
     *
     * See FiRa Consortium Common Service Management Layer Technical
     * Specification v1.0.0, Section 7.5.3.2, 'UWB Session Data structure',
     * Table 53, pages 103-107.
     */
    static constexpr auto DeviceRoleDefault = DeviceRole::Responder;
    static constexpr auto RangingConfigurationDefault = RangingConfiguration{ RangingMethod::DoubleSidedTwoWay, MeasurementReportMode::Deferred };
    static constexpr auto StsConfigurationDefault = StsConfiguration::Static;
    static constexpr auto MultiNodeModeDefault = MultiNodeMode::Unicast;
    static constexpr auto RangingTimeStructDefault = RangingMode::Block;
    static constexpr auto ScheduledModeDefault = SchedulingMode::Time;
    static constexpr auto HoppingModeDefault = false;
    static constexpr auto BlockStridingDefault = false;
    static constexpr auto UwbInitiationTimeDefault = 0UL;
    static constexpr auto ChannelDefault = Channel::C9;
    static constexpr auto RFrameConfigDefault = StsPacketConfiguration::SP3;
    static constexpr auto CcConstraintLengthDefault = ConvolutionalCodeConstraintLength::K3;
    static constexpr auto PrfModeDefault = PrfMode::Bprf;
    static constexpr auto Sp0PhySetNumberDefault = 1;
    static constexpr auto Sp1PhySetNumberDefault = 1;
    static constexpr auto Sp3PhySetNumberDefault = 0;
    static constexpr auto PreableCodeIndexDefault = 0;
    static constexpr auto MacAddressModeDefault = UwbMacAddressType::Short;
    static constexpr auto KeyRotationRateDefault = 0UL;
    static constexpr auto MacFcsTypeDefault = UwbMacAddressFcsType::Crc16;
    static constexpr auto MaxRrRetryDefault = 0U;
    static const std::unordered_set<ResultReportConfiguration> ResultReportConfigurationsDefault;

    /**
     * @brief Construct UwbConfiguration object with default values and settings.
     * TODO: make this c'tor private once refactoring is complete.
     */
    UwbConfiguration() = default;

    /**
     * @brief Creates a new UwbConfiguration builder object.
     * 
     * @return UwbConfiguration::Builder 
     */
    static UwbConfiguration::Builder
    Create() noexcept;

    /**
     * @brief Default equality operator.
     *
     * @param other
     * @return true
     * @return false
     */
    bool
    operator==(const UwbConfiguration& other) const noexcept = default;

    /**
     * @brief Convert this object into a FiRa Data Object (DO).
     *
     * @return std::unique_ptr<encoding::TlvBer>
     */
    std::unique_ptr<encoding::TlvBer>
    ToDataObject() const;

    /**
     * @brief Attempt to create a UwbConfiguration object from a TlvBer.
     *
     * @param tlv
     * @return UwbConfiguration
     */
    static UwbConfiguration
    FromDataObject(const encoding::TlvBer& tlv);

    uint32_t _firaPhyVersion{ 0 };
    uint32_t _firaMacVersion{ 0 };
    DeviceRole _deviceRole{ DeviceRoleDefault };
    RangingConfiguration _rangingConfiguration{ RangingConfigurationDefault };
    StsConfiguration _stsConfiguration{ StsConfigurationDefault };
    MultiNodeMode _multiNodeMode{ MultiNodeModeDefault };
    RangingMode _rangingTimeStruct{ RangingTimeStructDefault };
    SchedulingMode _schedulingMode{ ScheduledModeDefault };
    bool _hoppingMode{ HoppingModeDefault };
    bool _blockStriding{ BlockStridingDefault };
    uint32_t _uwbInitiationTime{ UwbInitiationTimeDefault };
    Channel _channel{ Channel::C9 };
    StsPacketConfiguration _rframeConfig{ RFrameConfigDefault };
    ConvolutionalCodeConstraintLength _convolutionalCodeConstraintLength{ CcConstraintLengthDefault };
    PrfMode _prfMode{ PrfModeDefault };
    uint8_t _sp0PhySetNumber{ Sp0PhySetNumberDefault };
    uint8_t _sp1PhySetNumber{ Sp0PhySetNumberDefault };
    uint8_t _sp3PhySetNumber{ Sp0PhySetNumberDefault };
    uint8_t _preableCodeIndex{ PreableCodeIndexDefault };
    std::unordered_set<ResultReportConfiguration> _resultReportConfigurations{ ResultReportConfigurationsDefault };
    UwbMacAddressType _macAddressMode{ MacAddressModeDefault };
    std::optional<UwbMacAddress> _controleeShortMacAddress;
    UwbMacAddress _controllerMacAddress;
    uint8_t _slotsPerRangingRound{ 0 };
    uint8_t _maxContentionPhaseLength{ 0 };
    uint8_t _slotDuration{ 0 };
    uint16_t _rangingInterval{ 0 };
    uint8_t _keyRotationRate{ KeyRotationRateDefault };
    UwbMacAddressFcsType _macAddressFcsType{ MacFcsTypeDefault };
    uint16_t _maxRangingRoundRetry{ MaxRrRetryDefault };

    std::optional<uint32_t>
    GetFiraPhyVersion() const noexcept;

    std::optional<uint32_t>
    GetFiraMacVersion() const noexcept;

    std::optional<uwb::protocol::fira::DeviceRole>
    GetDeviceRole() const noexcept;

    std::optional<uwb::protocol::fira::RangingConfiguration>
    GetRangingConfiguration() const noexcept;

    std::optional<uwb::protocol::fira::StsConfiguration>
    GetStsConfiguration() const noexcept;

    std::optional<uwb::protocol::fira::MultiNodeMode>
    GetMultiNodeMode() const noexcept;

    std::optional<uwb::protocol::fira::RangingMode>
    GetRangingTimeStruct() const noexcept;

    std::optional<uwb::protocol::fira::SchedulingMode>
    GetSchedulingMode() const noexcept;

    std::optional<bool>
    GetHoppingMode() const noexcept;

    std::optional<bool>
    GetBlockStriding() const noexcept;

    std::optional<uint32_t>
    GetUwbInitiationTime() const noexcept;

    std::optional<uwb::protocol::fira::Channel>
    GetChannel() const noexcept;

    std::optional<uwb::protocol::fira::StsPacketConfiguration>
    GetRFrameConfig() const noexcept;

    std::optional<uwb::protocol::fira::ConvolutionalCodeConstraintLength>
    GetConvolutionalCodeConstraintLength() const noexcept;

    std::optional<uwb::protocol::fira::PrfMode>
    GetPrfMode() const noexcept;

    std::optional<uint8_t>
    GetSp0PhySetNumber() const noexcept;

    std::optional<uint8_t>
    GetSp1PhySetNumber() const noexcept;

    std::optional<uint8_t>
    GetSp3PhySetNumber() const noexcept;

    std::optional<uint8_t>
    GetPreableCodeIndex() const noexcept;

    std::optional<std::unordered_set<uwb::protocol::fira::ResultReportConfiguration>>
    GetResultReportConfigurations() const noexcept;

    std::optional<uwb::UwbMacAddressType>
    GetMacAddressMode() const noexcept;

    std::optional<uwb::UwbMacAddress>
    GetControleeShortMacAddress() const noexcept;

    std::optional<uwb::UwbMacAddress>
    GetControllerMacAddress() const noexcept;

    std::optional<uint8_t>
    GetSlotsPerRangingRound() const noexcept;

    std::optional<uint8_t>
    GetMaxContentionPhaseLength() const noexcept;

    std::optional<uint8_t>
    GetSlotDuration() const noexcept;

    std::optional<uint16_t>
    GetRangingInterval() const noexcept;

    std::optional<uint8_t>
    GetKeyRotationRate() const noexcept;

    std::optional<uwb::UwbMacAddressFcsType>
    GetMacAddressFcsType() const noexcept;

    std::optional<uint16_t>
    GetMaxRangingRoundRetry() const noexcept;

private:
    std::optional<uint32_t> m_firaPhyVersion;
    std::optional<uint32_t> m_firaMacVersion;
    std::optional<uwb::protocol::fira::DeviceRole> m_deviceRole;
    std::optional<uwb::protocol::fira::RangingConfiguration> m_rangingConfiguration;
    std::optional<uwb::protocol::fira::StsConfiguration> m_stsConfiguration;
    std::optional<uwb::protocol::fira::MultiNodeMode> m_multiNodeMode;
    std::optional<uwb::protocol::fira::RangingMode> m_rangingTimeStruct;
    std::optional<uwb::protocol::fira::SchedulingMode> m_schedulingMode;
    std::optional<bool> m_hoppingMode;
    std::optional<bool> m_blockStriding;
    std::optional<uint32_t> m_uwbInitiationTime;
    std::optional<uwb::protocol::fira::Channel> m_channel;
    std::optional<uwb::protocol::fira::StsPacketConfiguration> m_rframeConfig;
    std::optional<uwb::protocol::fira::ConvolutionalCodeConstraintLength> m_convolutionalCodeConstraintLength;
    std::optional<uwb::protocol::fira::PrfMode> m_prfMode;
    std::optional<uint8_t> m_sp0PhySetNumber;
    std::optional<uint8_t> m_sp1PhySetNumber;
    std::optional<uint8_t> m_sp3PhySetNumber;
    std::optional<uint8_t> m_preableCodeIndex;
    std::optional<std::unordered_set<uwb::protocol::fira::ResultReportConfiguration>> m_resultReportConfigurations;
    std::optional<uwb::UwbMacAddressType> m_macAddressMode;
    std::optional<uwb::UwbMacAddress> m_controleeShortMacAddress;
    std::optional<uwb::UwbMacAddress> m_controllerMacAddress;
    std::optional<uint8_t> m_slotsPerRangingRound;
    std::optional<uint8_t> m_maxContentionPhaseLength;
    std::optional<uint8_t> m_slotDuration;
    std::optional<uint16_t> m_rangingInterval;
    std::optional<uint8_t> m_keyRotationRate;
    std::optional<uwb::UwbMacAddressFcsType> m_macAddressFcsType;
    std::optional<uint16_t> m_maxRangingRoundRetry;
};

} // namespace uwb::protocol::fira

namespace std
{
template <>
struct hash<uwb::protocol::fira::UwbConfiguration>
{
    std::size_t
    operator()(const uwb::protocol::fira::UwbConfiguration& uwbConfiguration) const noexcept
    {
        std::size_t value = 0;
        notstd::hash_combine(value,
            uwbConfiguration._firaPhyVersion,
            uwbConfiguration._firaMacVersion,
            uwbConfiguration._deviceRole,
            uwbConfiguration._rangingConfiguration,
            uwbConfiguration._stsConfiguration,
            uwbConfiguration._multiNodeMode,
            uwbConfiguration._rangingTimeStruct,
            uwbConfiguration._schedulingMode,
            uwbConfiguration._hoppingMode,
            uwbConfiguration._blockStriding,
            uwbConfiguration._uwbInitiationTime,
            uwbConfiguration._channel,
            uwbConfiguration._rframeConfig,
            uwbConfiguration._convolutionalCodeConstraintLength,
            uwbConfiguration._prfMode,
            uwbConfiguration._sp0PhySetNumber,
            uwbConfiguration._sp1PhySetNumber,
            uwbConfiguration._sp3PhySetNumber,
            uwbConfiguration._preableCodeIndex,
            notstd::hash_range(std::cbegin(uwbConfiguration._resultReportConfigurations), std::cend(uwbConfiguration._resultReportConfigurations)),
            uwbConfiguration._macAddressMode,
            uwbConfiguration._controleeShortMacAddress,
            uwbConfiguration._controllerMacAddress,
            uwbConfiguration._slotsPerRangingRound,
            uwbConfiguration._maxContentionPhaseLength,
            uwbConfiguration._slotDuration,
            uwbConfiguration._rangingInterval,
            uwbConfiguration._keyRotationRate,
            uwbConfiguration._macAddressFcsType,
            uwbConfiguration._maxRangingRoundRetry);
        return value;
    }
};
} // namespace std

#endif // UWB_CONFIGURATION_HXX
