
#ifndef NEAR_OBJECT_CLI_DATA_HXX
#define NEAR_OBJECT_CLI_DATA_HXX

#include <cstdint>
#include <optional>
#include <string>

#include <uwb/UwbMacAddress.hxx>
#include <uwb/protocols/fira/FiraDevice.hxx>
#include <uwb/protocols/fira/UwbConfiguration.hxx>
#include <uwb/protocols/fira/UwbSessionData.hxx>

namespace nearobject::cli
{
/**
 * @brief Helper structure to hold parameters for a ranging session.
 */
struct UwbRangingParameters
{
    uint32_t SessionId;
    std::vector<uwb::protocol::fira::UwbApplicationConfigurationParameter> ApplicationConfigurationParameters;
};

/**
 * @brief Helper sturcture for staging UwbConfiguration data fields from CLI
 * input. This is an intermediary structure that allows native use with the
 * CLI11 parsing library.
 */
struct UwbConfigurationData
{
    // Mirrored properties from UwbConfiguration data. Any newly added fields
    // that should be supported from the command-line must also be added here,
    // along with parsing support in a NearObjectCli instance.
    std::optional<uwb::protocol::fira::DeviceRole> deviceRole;
    std::optional<uwb::protocol::fira::RangingDirection> rangingDirection;
    std::optional<uwb::protocol::fira::MeasurementReportMode> rangingMeasurementReportMode;
    std::optional<uwb::protocol::fira::StsConfiguration> stsConfiguration;
    std::optional<uwb::protocol::fira::MultiNodeMode> multiNodeMode;
    std::optional<uwb::protocol::fira::RangingMode> rangingTimeStruct;
    std::optional<uwb::protocol::fira::SchedulingMode> schedulingMode;
    std::optional<bool> hoppingMode;
    std::optional<bool> blockStriding;
    std::optional<uint32_t> uwbInitiationTime;
    std::optional<uwb::protocol::fira::Channel> channel;
    std::optional<uwb::protocol::fira::RFrameConfiguration> rframeConfig;
    std::optional<uwb::protocol::fira::ConvolutionalCodeConstraintLength> convolutionalCodeConstraintLength;
    std::optional<uwb::protocol::fira::PrfMode> prfMode;
    std::optional<uint8_t> sp0PhySetNumber;
    std::optional<uint8_t> sp1PhySetNumber;
    std::optional<uint8_t> sp3PhySetNumber;
    std::optional<uint8_t> preambleCodeIndex;
    std::optional<uwb::UwbMacAddressType> macAddressMode;
    std::optional<uwb::UwbMacAddress> controllerMacAddress;
    std::optional<uwb::UwbMacAddress> controleeShortMacAddress;
    std::optional<uint8_t> slotsPerRangingRound;
    std::optional<uint8_t> maxContentionPhaseLength;
    std::optional<uint16_t> slotDuration;
    std::optional<uint16_t> rangingInterval;
    std::optional<uint8_t> keyRotationRate;
    std::optional<uwb::UwbMacAddressFcsType> macAddressFcsType;
    std::optional<uint16_t> maxRangingRoundRetry;
    std::optional<uint8_t> numberOfControlees;
    std::optional<uwb::protocol::fira::DeviceType> deviceType;

    // Free-form string arugments that will be manually parsed to their target format(s).
    std::string firaPhyVersionString;
    std::string firaMacVersionString;
    std::string resultReportConfigurationString;

    /**
     * @brief Convert the staging data to a complete UwbConfiguration object.
     *
     * @return uwb::protocol::fira::UwbConfiguration
     */
    operator uwb::protocol::fira::UwbConfiguration() const noexcept;
};

/**
 * @brief Helper structure for staging UwbApplicationConfigurationParameter data fields from CLI
 * input. This is an intermediary structure that allows native use with the
 * CLI11 parsing library.
 */
struct UwbApplicationConfigurationParameterData
{
    // Mirrored properties from UwbApplicationConfigurationParameter data. Any newly added fields
    // that should be supported from the command-line must also be added here,
    // along with parsing support in a NearObjectCli instance.
    std::optional<uwb::protocol::fira::DeviceType> deviceType;
    std::optional<uwb::protocol::fira::RangingRoundUsage> rangingRoundUsage;
    std::optional<uwb::protocol::fira::StsConfiguration> stsConfiguration;
    std::optional<uwb::protocol::fira::MultiNodeMode> multiNodeMode;
    std::optional<uwb::protocol::fira::Channel> channelNumber;
    std::optional<uint8_t> numberOfControlees;
    std::optional<uwb::UwbMacAddress> deviceMacAddress;
    std::optional<std::unordered_set<uwb::UwbMacAddress>> destinationMacAddresses;
    std::optional<uint16_t> slotDuration;
    std::optional<uint32_t> rangingInterval;
    std::optional<uint32_t> stsIndex;
    std::optional<uwb::UwbMacAddressFcsType> macFcsType;
    std::optional<uwb::protocol::fira::RangingRoundControl> rangingRoundControl;
    std::optional<uwb::protocol::fira::AoAResult> aoaResultRequest;
    std::optional<uwb::protocol::fira::RangeDataNotificationConfiguration> rangeDataNotificationConfig;
    std::optional<uint16_t> rangeDataNotificationProximityNear;
    std::optional<uint16_t> rangeDataNotificationProximityFar;
    std::optional<uwb::protocol::fira::DeviceRole> deviceRole;
    std::optional<uwb::protocol::fira::RFrameConfiguration> rFrameConfiguration;
    std::optional<uint8_t> preambleCodeIndex;
    std::optional<uint8_t> sfdId;
    std::optional<uwb::protocol::fira::PsduDataRate> psduDataRate;
    std::optional<uwb::protocol::fira::PreambleDuration> preambleDuration;
    std::optional<uwb::protocol::fira::RangingMode> rangingTimeStruct;
    std::optional<uint8_t> slotsPerRangingRound;
    std::optional<uwb::protocol::fira::TxAdaptivePayloadPower> txAdaptivePayloadPower;
    std::optional<uint8_t> responderSlotIndex;
    std::optional<uwb::protocol::fira::PrfModeDetailed> prfMode;
    std::optional<uwb::protocol::fira::SchedulingMode> scheduledMode;
    std::optional<uwb::protocol::fira::KeyRotation> keyRotation;
    std::optional<uint8_t> keyRotationRate;
    std::optional<uint8_t> sessionPriority;
    std::optional<uwb::UwbMacAddressType> macAddressMode;
    std::optional<uint16_t> vendorId;
    std::optional<std::array<uint8_t, uwb::protocol::fira::StaticStsInitializationVectorLength>> staticStsIv;
    std::optional<uint8_t> numberOfStsSegments;
    std::optional<uint16_t> maxRangingRoundRetry;
    std::optional<uint32_t> uwbInitiationTime;
    std::optional<bool> hoppingMode;
    std::optional<uint8_t> blockStrideLength;
    std::optional<std::unordered_set<uwb::protocol::fira::ResultReportConfiguration>> resultReportConfig;
    std::optional<uint8_t> inBandTerminationAttemptCount;
    std::optional<uint32_t> subSessionId;
    std::optional<uwb::protocol::fira::BprfPhrDataRate> bprfPhrDataRate;
    std::optional<uint16_t> maxNumberOfMeasurements;
    std::optional<uwb::protocol::fira::StsLength> stsLength;

    /**
     * @brief Map of application configuration parameter type to the parameter's value.
     */
    std::unordered_map<uwb::protocol::fira::UwbApplicationConfigurationParameterType, uwb::protocol::fira::UwbApplicationConfigurationParameterValue>
    GetValueMap() const;
};

/**
 * @brief Base class for data parsed by NearObjectCli.
 */
struct NearObjectCliData
{
    virtual ~NearObjectCliData() = default;

    std::string deviceMacAddressString;
    std::string destinationMacAddressesString;
    std::string resultReportConfigurationString;
    UwbConfigurationData uwbConfiguration{};
    UwbApplicationConfigurationParameterData applicationConfigurationParametersData{};
    uwb::protocol::fira::StaticRangingInfo StaticRanging{};
    uwb::protocol::fira::UwbSessionData SessionData{};
    UwbRangingParameters RangingParameters{};
    uint32_t SessionId{ 0 };
};

} // namespace nearobject::cli

#endif // NEAR_OBJECT_CLI_HXX
