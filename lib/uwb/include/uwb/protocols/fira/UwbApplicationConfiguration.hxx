
#ifndef UWB_APPLICATION_CONFIGURATION_HXX
#define UWB_APPLICATION_CONFIGURATION_HXX

#include <bitset>
#include <cstdint>
#include <vector>

#include <array>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <variant>

#include <uwb/protocols/fira/FiraDevice.hxx>
#include <uwb/protocols/fira/RangingMethod.hxx>
#include <uwb/protocols/fira/StaticRangingInfo.hxx>
#include <uwb/UwbMacAddress.hxx>

namespace uwb::protocol::fira
{
enum class UwbApplicationConfigurationParameterType {
    DeviceType,
    // TODO: replace with NOF definitions when moved to repo
};

struct UwbApplicationConfiguration
{
    DeviceType DevType{ DeviceType::Controller };
    RangingMethod RangingSessionMethod{}; // TODO: fix me
    StsConfiguration StsConfig{ StsConfiguration::Static };
    MultiNodeMode RangingModeMultiNode{ MultiNodeMode::Unicast };
    Channel ChannelNumber{ Channel::C9 };
    ::uwb::UwbMacAddress DeviceMacAddress{};
    // TODO: attempt to combine these next 2
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
