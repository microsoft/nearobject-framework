
#ifndef FIRA_UWB_CAPABILITY_HXX
#define FIRA_UWB_CAPABILITY_HXX

#include <cstdint>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

#include <notstd/hash.hxx>

#include <TlvBer.hxx>
#include <uwb/protocols/fira/FiraDevice.hxx>
#include <uwb/protocols/fira/RangingConfiguration.hxx>

namespace uwb::protocol::fira
{
struct UwbCapability
{
    struct IncorrectNumberOfBytesInValueError : public std::exception
    {};
    struct IncorrectNumberOfBytesInTagError : public std::exception
    {};
    struct IncorrectTlvTag : public std::exception
    {};

    /**
     * @brief See FiRa Consortium Common Service Management Layer Technical
     * Specification v1.0.0, Section 7.5.3.2, 'UWB Controlee Info', Table 52,
     * pages 96-99.
     */
    static constexpr uint8_t Tag = 0xA3;

    /**
     * @brief See FiRa Consortium Common Service Management Layer Technical
     * Specification v1.0.0, Section 7.5.3.2, 'UWB Controlee Info', Table 52,
     * pages 96-99.
     */
    enum class ParameterTag : uint8_t {
        FiraPhyVersionRange = 0x80,
        FiraMacVersionRange = 0x81,
        DeviceRoles = 0x82,
        RangingMethod = 0x83,
        StsConfig = 0x84,
        MultiNodeMode = 0x85,
        RangingMode = 0x86,
        ScheduledMode = 0x87,
        HoppingMode = 0x88,
        BlockStriding = 0x89,
        UwbInitiationTime = 0x8A,
        Channels = 0x8B,
        RFrameConfig = 0x8C,
        CcConstraintLength = 0x8D,
        BprfParameterSets = 0x8E,
        HprfParameterSets = 0x8F,
        AoaSupport = 0x90,
        ExtendedMacAddress = 0x91,
    };

    static const std::array<ParameterTag, 18> ParameterTags;

    static constexpr auto MultiNodeModesDefault = {
        MultiNodeMode::Unicast,
        MultiNodeMode::OneToMany,
    };

    static constexpr auto DeviceRolesDefault = {
        DeviceRole::Initiator,
        DeviceRole::Responder,
    };

    static constexpr auto StsConfigurationsDefault = {
        StsConfiguration::Static,
        StsConfiguration::Dynamic,
        StsConfiguration::DynamicWithResponderSubSessionKey,
    };

    static constexpr auto RFrameConfigurationsDefault = {
        StsPacketConfiguration::SP1,
        StsPacketConfiguration::SP3,
    };

    // TODO: spec says "if omitted, AoA is supported". We've assumed "all"
    // support, but this could be wrong. Double-check this with FiRa directly.
    static constexpr auto AngleOfArrivalTypesDefault = {
        AngleOfArrival::Azimuth90,
        AngleOfArrival::Azimuth180,
        AngleOfArrival::Elevation,
    };

    static constexpr auto SchedulingModeTypesDefault = {
        SchedulingMode::Time,
    };

    static constexpr auto RangingTimeStructsDefault = {
        RangingMode::Block,
    };

    static constexpr auto ConvolutionalCodeConstraintLengthsDefault = {
        ConvolutionalCodeConstraintLength::K3,
        ConvolutionalCodeConstraintLength::K7,
    };

    static constexpr auto ChannelsDefault = {
        Channel::C5,
        Channel::C6,
        Channel::C8,
        Channel::C9,
        Channel::C10,
        Channel::C12,
        Channel::C13,
        Channel::C14,
    };

    static constexpr auto BprfParameterSetsDefault = {
        BprfParameter::Set1,
        BprfParameter::Set2,
        BprfParameter::Set3,
        BprfParameter::Set4,
        BprfParameter::Set5,
        BprfParameter::Set6,
    };

    static constexpr auto HprfParameterSetsDefault = {
        HprfParameter::Set1,
        HprfParameter::Set2,
        HprfParameter::Set3,
        HprfParameter::Set4,
        HprfParameter::Set5,
        HprfParameter::Set6,
        HprfParameter::Set7,
        HprfParameter::Set8,
        HprfParameter::Set9,
        HprfParameter::Set10,
        HprfParameter::Set11,
        HprfParameter::Set12,
        HprfParameter::Set13,
        HprfParameter::Set14,
        HprfParameter::Set15,
        HprfParameter::Set16,
        HprfParameter::Set17,
        HprfParameter::Set18,
        HprfParameter::Set19,
        HprfParameter::Set20,
        HprfParameter::Set21,
        HprfParameter::Set22,
        HprfParameter::Set23,
        HprfParameter::Set24,
        HprfParameter::Set25,
        HprfParameter::Set26,
        HprfParameter::Set27,
        HprfParameter::Set28,
        HprfParameter::Set29,
        HprfParameter::Set30,
        HprfParameter::Set31,
        HprfParameter::Set32,
        HprfParameter::Set33,
        HprfParameter::Set34,
        HprfParameter::Set35,
    };

    static const std::initializer_list<RangingConfiguration> RangingConfigurationsDefault;

    static const std::unordered_map<MultiNodeMode, std::size_t> MultiNodeModeBit;
    static const std::unordered_map<DeviceRole, std::size_t> DeviceRoleBit;
    static const std::unordered_map<StsConfiguration, std::size_t> StsConfigurationBit;
    static const std::unordered_map<StsPacketConfiguration, std::size_t> RFrameConfigurationBit;
    static const std::unordered_map<AngleOfArrival, std::size_t> AngleOfArrivalBit;
    static const std::unordered_map<SchedulingMode, std::size_t> SchedulingModeBit;
    static const std::unordered_map<RangingMode, std::size_t> RangingModeBit;
    static const std::unordered_map<RangingConfiguration, std::size_t> RangingConfigurationBit;
    static const std::unordered_map<ConvolutionalCodeConstraintLength, std::size_t> ConvolutionalCodeConstraintLengthsBit;
    static const std::unordered_map<Channel, std::size_t> ChannelsBit;
    static const std::unordered_map<BprfParameter, std::size_t> BprfParameterSetsBit;
    static const std::unordered_map<HprfParameter, std::size_t> HprfParameterSetsBit;
    static constexpr std::size_t AngleOfArrivalFomBit = 3;
    static constexpr std::size_t BlockStridingBit = 0;
    static constexpr std::size_t HoppingModeBit = 0;

    uint32_t FiraPhyVersionRange{ 0 };
    uint32_t FiraMacVersionRange{ 0 };
    bool ExtendedMacAddress{ false };
    bool UwbInitiationTime{ false };
    bool AngleOfArrivalFom{ false };
    bool BlockStriding{ true };
    bool HoppingMode{ true };
    std::vector<MultiNodeMode> MultiNodeModes{ MultiNodeModesDefault };
    std::vector<DeviceRole> DeviceRoles{ DeviceRolesDefault };
    std::vector<StsConfiguration> StsConfigurations{ StsConfigurationsDefault };
    std::vector<StsPacketConfiguration> RFrameConfigurations{ RFrameConfigurationsDefault };
    std::vector<AngleOfArrival> AngleOfArrivalTypes{ AngleOfArrivalTypesDefault };
    std::vector<SchedulingMode> SchedulingModes{ SchedulingModeTypesDefault };
    std::vector<RangingMode> RangingTimeStructs{ RangingTimeStructsDefault };
    std::vector<RangingConfiguration> RangingConfigurations{ RangingConfigurationsDefault };
    std::vector<ConvolutionalCodeConstraintLength> ConvolutionalCodeConstraintLengths{ ConvolutionalCodeConstraintLengthsDefault };
    std::vector<Channel> Channels{ ChannelsDefault };
    std::vector<BprfParameter> BprfParameterSets{ BprfParameterSetsDefault };
    std::vector<HprfParameter> HprfParameterSets{ HprfParameterSetsDefault };

    /**
     * @brief Convert this object into a FiRa Data Object (DO).
     *
     * @return encoding::TlvBer
     */
    std::unique_ptr<encoding::TlvBer>
    ToOobDataObject() const;

    /**
     * @brief
     *
     * @return UwbCapability
     */
    static UwbCapability
    FromOobDataObject(const encoding::TlvBer& tlv);
};

bool
operator==(const UwbCapability& lhs, const UwbCapability& rhs) noexcept;

bool
operator!=(const UwbCapability& lhs, const UwbCapability& rhs) noexcept;

} // namespace uwb::protocol::fira

namespace std
{
template <>
struct hash<uwb::protocol::fira::UwbCapability>
{
    std::size_t
    operator()(const uwb::protocol::fira::UwbCapability& uwbCapability) const noexcept
    {
        std::size_t value = 0;
        notstd::hash_combine(value,
            uwbCapability.FiraPhyVersionRange,
            uwbCapability.FiraMacVersionRange,
            uwbCapability.ExtendedMacAddress,
            uwbCapability.UwbInitiationTime,
            uwbCapability.AngleOfArrivalFom,
            uwbCapability.BlockStriding,
            uwbCapability.HoppingMode,
            notstd::hash_range(std::cbegin(uwbCapability.RangingConfigurations), std::cend(uwbCapability.RangingConfigurations)),
            notstd::hash_range(std::cbegin(uwbCapability.MultiNodeModes), std::cend(uwbCapability.MultiNodeModes)),
            notstd::hash_range(std::cbegin(uwbCapability.DeviceRoles), std::cend(uwbCapability.DeviceRoles)),
            notstd::hash_range(std::cbegin(uwbCapability.StsConfigurations), std::cend(uwbCapability.StsConfigurations)),
            notstd::hash_range(std::cbegin(uwbCapability.RFrameConfigurations), std::cend(uwbCapability.RFrameConfigurations)),
            notstd::hash_range(std::cbegin(uwbCapability.AngleOfArrivalTypes), std::cend(uwbCapability.AngleOfArrivalTypes)),
            notstd::hash_range(std::cbegin(uwbCapability.SchedulingModes), std::cend(uwbCapability.SchedulingModes)),
            notstd::hash_range(std::cbegin(uwbCapability.RangingTimeStructs), std::cend(uwbCapability.RangingTimeStructs)),
            notstd::hash_range(std::cbegin(uwbCapability.ConvolutionalCodeConstraintLengths), std::cend(uwbCapability.ConvolutionalCodeConstraintLengths)),
            notstd::hash_range(std::cbegin(uwbCapability.Channels), std::cend(uwbCapability.Channels)),
            notstd::hash_range(std::cbegin(uwbCapability.BprfParameterSets), std::cend(uwbCapability.BprfParameterSets)),
            notstd::hash_range(std::cbegin(uwbCapability.HprfParameterSets), std::cend(uwbCapability.HprfParameterSets)));
        return value;
    }
};
} // namespace std

#endif // FIRA_UWB_CAPABILITY_HXX
