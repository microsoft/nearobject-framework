
#ifndef UWB_CONFIGURATION_HXX
#define UWB_CONFIGURATION_HXX

#include <compare>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <unordered_set>
#include <variant>

#include <TlvBer.hxx>
#include <notstd/hash.hxx>

#include <uwb/UwbMacAddress.hxx>
#include <uwb/protocols/fira/FiraDevice.hxx>
#include <uwb/protocols/fira/RangingMethod.hxx>

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
        PreableCodeIndex = 0x92, // TODO fix the typo here
        ResultReportConfig = 0x93,
        MacAddressMode = 0x94,
        ControleeShortMacAddress = 0x95,
        ControllerMacAddress = 0x96,
        SlotsPerRr = 0x97,
        MaxContentionPhaseLength = 0x98,
        SlotDuration = 0x99,
        RangingInterval = 0x9A,
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
    static constexpr auto RangingMethodDefault = RangingMethod{ RangingDirection::DoubleSidedTwoWay, MeasurementReportMode::Deferred };
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
     * @brief Variant for all possible property types.
     */
    using ParameterTypesVariant = std::variant<
        bool,
        uint8_t,
        uint16_t,
        uint32_t,
        uwb::protocol::fira::Channel,
        uwb::protocol::fira::ConvolutionalCodeConstraintLength,
        uwb::protocol::fira::DeviceRole,
        uwb::protocol::fira::MultiNodeMode,
        uwb::protocol::fira::PrfMode,
        uwb::protocol::fira::RangingMethod,
        uwb::protocol::fira::RangingMode,
        uwb::protocol::fira::ResultReportConfiguration,
        uwb::protocol::fira::SchedulingMode,
        uwb::protocol::fira::StsConfiguration,
        uwb::protocol::fira::StsPacketConfiguration,
        uwb::UwbMacAddress,
        uwb::UwbMacAddressFcsType,
        uwb::UwbMacAddressType,
        std::unordered_set<uwb::protocol::fira::ResultReportConfiguration>>;

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

    /**
     * @brief The map of parameter tags and their values from the configuration object.
     *
     * @return const std::unordered_map<uwb::protocol::fira::UwbConfiguration::ParameterTag, ParameterTypesVariant>&
     */
    const std::unordered_map<uwb::protocol::fira::UwbConfiguration::ParameterTag, ParameterTypesVariant>&
    GetValueMap() const noexcept;

    std::optional<uint32_t>
    GetFiraPhyVersion() const noexcept;

    std::optional<uint32_t>
    GetFiraMacVersion() const noexcept;

    std::optional<uwb::protocol::fira::DeviceRole>
    GetDeviceRole() const noexcept;

    std::optional<uwb::protocol::fira::RangingMethod>
    GetRangingMethod() const noexcept;

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

    std::unordered_set<uwb::protocol::fira::ResultReportConfiguration>
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
    /**
     * @brief Helper function to resolve and obtain a value, given a parameter
     * tag.
     *
     * @tparam T
     * @param tag
     * @return std::optional<T> A constructed value if specified, std::nullopt
     * otherwise.
     */
    template <typename T>
    std::optional<T>
    GetValue(ParameterTag tag) const noexcept
    {
        auto it = m_values.find(tag);
        return (it != std::cend(m_values))
            ? std::optional<T>(std::get<T>(it->second))
            : std::nullopt;
    }

private:
    std::unordered_map<ParameterTag, ParameterTypesVariant> m_values{};
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
        auto resultReportConfigurations = uwbConfiguration.GetResultReportConfigurations();
        notstd::hash_combine(value,
            uwbConfiguration.GetFiraPhyVersion(),
            uwbConfiguration.GetFiraMacVersion(),
            uwbConfiguration.GetDeviceRole(),
            uwbConfiguration.GetRangingMethod(),
            uwbConfiguration.GetStsConfiguration(),
            uwbConfiguration.GetMultiNodeMode(),
            uwbConfiguration.GetRangingTimeStruct(),
            uwbConfiguration.GetSchedulingMode(),
            uwbConfiguration.GetHoppingMode(),
            uwbConfiguration.GetBlockStriding(),
            uwbConfiguration.GetUwbInitiationTime(),
            uwbConfiguration.GetChannel(),
            uwbConfiguration.GetRFrameConfig(),
            uwbConfiguration.GetConvolutionalCodeConstraintLength(),
            uwbConfiguration.GetPrfMode(),
            uwbConfiguration.GetSp0PhySetNumber(),
            uwbConfiguration.GetSp1PhySetNumber(),
            uwbConfiguration.GetSp3PhySetNumber(),
            uwbConfiguration.GetPreableCodeIndex(),
            notstd::hash_range(std::cbegin(resultReportConfigurations), std::cend(resultReportConfigurations)),
            uwbConfiguration.GetMacAddressMode(),
            uwbConfiguration.GetControleeShortMacAddress(),
            uwbConfiguration.GetControllerMacAddress(),
            uwbConfiguration.GetSlotsPerRangingRound(),
            uwbConfiguration.GetMaxContentionPhaseLength(),
            uwbConfiguration.GetSlotDuration(),
            uwbConfiguration.GetRangingInterval(),
            uwbConfiguration.GetKeyRotationRate(),
            uwbConfiguration.GetMacAddressFcsType(),
            uwbConfiguration.GetMaxRangingRoundRetry());
        return value;
    }
};
} // namespace std

#endif // UWB_CONFIGURATION_HXX
