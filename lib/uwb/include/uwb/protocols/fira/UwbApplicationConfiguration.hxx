
#ifndef UWB_APPLICATION_CONFIGURATION_HXX
#define UWB_APPLICATION_CONFIGURATION_HXX

#include <bitset>
#include <cstdint>
#include <unordered_set>
#include <variant>
#include <vector>

#include <uwb/UwbMacAddress.hxx>
#include <uwb/protocols/fira/FiraDevice.hxx>
#include <uwb/protocols/fira/RangingMethod.hxx>
#include <uwb/protocols/fira/StaticRangingInfo.hxx>

namespace uwb::protocol::fira
{
/**
 * @brief See FiRa Consortium - UCI Generic Specification v1.1.0, Section 8.3,
 * Table 29, 'APP Configuration Parameter IDs'.
 */
enum class UwbApplicationConfigurationParameterType : uint8_t {
    DeviceType = 0x00U,
    RangingRoundUsage = 0x01U,
    StsConfiguration = 0x02U,
    MultiNodeMode = 0x03U,
    ChannelNumber = 0x04U,
    NumberOfControlees = 0x05U,
    DeviceMacAddress = 0x06U,
    DestinationMacAddresses = 0x07U,
    SlotDuration = 0x08U,
    RangingInterval = 0x09U,
    StsIndex = 0x0AU,
    MacFcsType = 0x0BU,
    RangingRoundControl = 0x0CU,
    AoAResultRequest = 0x0DU,
    RangeDataNotificationConfig = 0x0EU,
    RangeDataNotificationProximityNear = 0x0FU,
    RangeDataNotificationProximityFar = 0x10U,
    DeviceRole = 0x11U,
    RFrameConfiguration = 0x12U,
    PreambleCodeIndex = 0x14U,
    SfdId = 0x15U,
    PsduDataRate = 0x16U,
    PreambleDuration = 0x17U,
    RangingTimeStruct = 0x1AU,
    SlotsPerRangingRound = 0x1BU,
    TxAdaptivePayloadPower = 0x1CU,
    ResponderSlotIndex = 0x1EU,
    PrfMode = 0x1FU,
    ScheduledMode = 0x22U,
    KeyRotation = 0x23U,
    KeyRotationRate = 0x24U,
    SessionPriority = 0x25U,
    MacAddressMode = 0x26U,
    VendorId = 0x27U,
    StaticStsIv = 0x28U,
    NumberOfStsSegments = 0x29U,
    MaxRangingRoundRetry = 0x2AU,
    UwbInitiationTime = 0x2BU,
    HoppingMode = 0x2CU,
    BlockStrideLength = 0x2DU,
    ResultReportConfig = 0x2EU,
    InBandTerminationAttemptCount = 0x2FU,
    SubSessionId = 0x30U,
    BprfPhrDataRate = 0x31U,
    MaxNumberOfMeasurements = 0x32U,
    StsLength = 0x35U,
};

using UwbApplicationConfigurationParameterValue = std::variant<
    bool, // HOPPING_MODE, tag 0x2C, size 1
    uint8_t, // NUMBER_OF_CONTROLLEES, tag 0x05, PREAMBLE_CODE_INDEX, tag 0x14, SFD_ID, tag 0x15, SLOTS_PER_RR, tag 0x1B, RESPONDER_SLOT_INDEX, tag 0x1E, KEY_ROTATION_RATE, tag 0x24, SESSION_PRIORITY, tag 0x25, NUMBER_OF_STS_SEGMENTS, tag 0x29, BLOCK_STRIDE_LENGTH, tag 0x2D, IN_BAND_TERMMINATION_ATTEMPT_COUNT, tag 0x2F, size 1
    uint16_t, // SLOT_DURATION, tag 0x08, RANGE_DATA_NTF_PROXIMITY_NEAR, tag 0x0F, RANGE_DATA_NTF_PROXIMITY_FAR, tag 0x10, VENDOR_ID, tag 0x27, MAX_RR_RETRY, tag 0x2A, MAX_NUMBER_OF_MEASUREMENTS, tag 0x32, size 2
    uint32_t, // RANGING_INTERVAL, tag 0x09, STS_INDEX, tag 0x0A, UWB_INITIATION_TIME, tag 0x2B, SUB_SESSION_ID, tag 0x30, size 4
    ::uwb::protocol::fira::AoAResult, // AOA_RESULT_REQ, tag 0x0D, size 1
    ::uwb::protocol::fira::BprfPhrDataRate, // BPRF_PHR_DATA_RATE, tag 0x31, size 1
    ::uwb::protocol::fira::Channel, // CHANNEL_NUMBER, tag 0x04, size 1
    ::uwb::protocol::fira::DeviceRole, // DEVICE_ROLE, tag 0x11, size 1
    ::uwb::protocol::fira::DeviceType, // DEVICE_TYPE, tag 0x00, size 1
    ::uwb::protocol::fira::KeyRotation, // KEY_ROTATION, tag 0x23, size 1
    ::uwb::protocol::fira::MultiNodeMode, // MULTI_NODE_MODE, tag 0x03, size 1
    ::uwb::protocol::fira::PreambleDuration, // PREAMBLE_DURATION, tag 0x17, size 1
    ::uwb::protocol::fira::PrfMode, // PRF_MODE, tag 0x1F, size 1
    ::uwb::protocol::fira::PsduDataRate, // PSDU_DATA_RATE, tag 0x16, size 1
    ::uwb::protocol::fira::RangeDataNotificationConfiguration, // RANGE_DATA_NTF_CONFIG, tag 0x0E, size 1
    ::uwb::protocol::fira::RangingRoundUsage, // RANGING_ROUND_USAGE, tag0x01, size 1
    ::uwb::protocol::fira::RangingMode, // RANGING_TIME_STRUCT, tag 0x1A, size 1
    ::uwb::protocol::fira::RangingRoundControl, // RANGING_ROUND_CONTROL, tag 0x0C, size 1,
    ::uwb::protocol::fira::ResultReportConfiguration, // RESULT_REPORT_CONFIG, tag 0x2E, size 1
    ::uwb::protocol::fira::SchedulingMode, // SCHEDULED_MODE, tag 0x22, size 1
    ::uwb::protocol::fira::StsConfiguration, // STS_CONFIG, tag 0x02, size 1
    ::uwb::protocol::fira::StsLength, // STS_LENGTH, tag 0x035, length 1,
    ::uwb::protocol::fira::StsPacketConfiguration, // RFRAME_CONFIG, tag 0x12, size 1
    ::uwb::protocol::fira::TxAdaptivePayloadPower, // TX_ADAPTIVE_PAYLOAD_POWER, tag 0x1C, size 1
    ::uwb::UwbMacAddress, // DEVICE_MAC_ADDRESS, tag 0x06, size 2/8
    ::uwb::UwbMacAddressFcsType, // MAC_FCS_TYPE, tag 0x0B, size 1
    ::uwb::UwbMacAddressType, // MAC_ADDRESS_MODE, tag 0x26, size 1
    std::array<uint8_t, StaticRangingInfo::InitializationVectorLength>, // STATIC_STS_IV, tag 0x28, size 6
    std::unordered_set<::uwb::UwbMacAddress>>; // DST_MAC_ADDRESS, tag 0x07, size 2/8*N

/**
 * @brief Session configuration controlled by the application.
 *
 * TODO: consider converting enumerations whose values are exclusively binary
 * (Disable, Enable) to booleans instead since the conversion can be done
 * directly with uary operators in both directions.
 */
struct UwbApplicationConfiguration
{
    DeviceType DevType{ DeviceType::Controller };
    RangingMethod RangingSessionMethod{}; // TODO: fix me
    StsConfiguration StsConfig{ StsConfiguration::Static };
    MultiNodeMode RangingModeMultiNode{ MultiNodeMode::Unicast };
    Channel ChannelNumber{ Channel::C9 };
    ::uwb::UwbMacAddress DeviceMacAddress{};
    uint8_t NumberOfControlees{ 0 };
    std::vector<::uwb::UwbMacAddress> Controlees{};
    uint16_t SlotDuration{ 2400U };
    uint32_t RangingInterval{ 200U };
    uint32_t StsIndex{ 0x00000000U };
    ::uwb::UwbMacAddressFcsType MacAddressFcsType{ ::uwb::UwbMacAddressFcsType::Crc16 };
    std::bitset<8> RangingRoundControl{ 0x03 };
    AoAResult AoAResultRequest{ AoAResult::Enable };
    RangeDataNotificationConfiguration RangeDataNotificationConfig{ RangeDataNotificationConfiguration::Enable };
    uint16_t RangeDataNotificationProximityNear{ 0U };
    uint16_t RangeDataNotificationProximityFar{ 2000U };
    DeviceRole Role{ DeviceRole::Initiator };
    StsPacketConfiguration RFrameConfiguration{ StsPacketConfiguration::SP3 };
    uint8_t PreambleCodeIndex{ 10 };
    uint8_t SfdId{ 2 };
    uint8_t PsduDataRate{ 0x00U };
    PreambleDuration PreambleDurationTime{ PreambleDuration::Symbols64 };
    RangingMode RangingModeTimeStructure{ RangingMode::Block };
    uint8_t SlotsPerRangingRound{ 25U };
    TxAdaptivePayloadPower TxAdaptivePayloadPowerSetting{ TxAdaptivePayloadPower::Disable };
    uint8_t ResponderSlotIndex{ 0x01U };
    SchedulingMode ScheduledMode{ SchedulingMode::Time };
    KeyRotation KeyRotationSetting{ KeyRotation::Disable };
    uint8_t KeyRotationRate{ 0x00U };
    uint8_t SessionPriority{ 50 };
    ::uwb::UwbMacAddressType MacAddressMode{ ::uwb::UwbMacAddressType::Short };
    StaticRangingInfo RangingInfoStatic{};
    uint8_t NumberOfStsSegments{ 0x01U };
    uint16_t MaxRangingRoundRetries{ 0U };
    bool HoppingModeEnabled{ false };
    uint8_t BlockStrideLength{ 0x00U };
    std::bitset<8> ResultReportConfiguration{ 0b00000001U };
    uint8_t InBandTerminationAttemptCount{ 0x01U };
    uint32_t SubSessionId{ 0U };
    BprfPhrDataRate BprfPhyHeaderDataRate{ BprfPhrDataRate::Rate850kbps };
    uint16_t MaxNumberOfMeasurements{ 0x0000U };
    StsLength StsSegmentLength{ StsLength::Symbols64 };
};
} // namespace uwb::protocol::fira

#endif // UWB_APPLICATION_CONFIGURATION_HXX
