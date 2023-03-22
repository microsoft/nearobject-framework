
#include <uwb/protocols/fira/UwbOobConversions.hxx>

using namespace uwb::protocol::fira;
/**
 * @brief map of uci tag to a function of how to generate that uci tag given UwbConfiguration
 *
 */
const std::unordered_map<UwbApplicationConfigurationParameterType, std::function<std::optional<UwbApplicationConfigurationParameterValue>(const UwbConfiguration&, DeviceType)>> uciGenerators{
    // params that directly transfer
    { UwbApplicationConfigurationParameterType::DeviceType, [](const UwbConfiguration& config, DeviceType deviceType) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return deviceType;
     } },
    { UwbApplicationConfigurationParameterType::DeviceRole, [](const UwbConfiguration& config, DeviceType deviceType) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetDeviceRole();
     } },
    { UwbApplicationConfigurationParameterType::StsConfiguration, [](const UwbConfiguration& config, DeviceType deviceType) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetStsConfiguration();
     } },
    { UwbApplicationConfigurationParameterType::MultiNodeMode, [](const UwbConfiguration& config, DeviceType deviceType) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetMultiNodeMode();
     } },
    { UwbApplicationConfigurationParameterType::ChannelNumber, [](const UwbConfiguration& config, DeviceType deviceType) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetChannel();
     } },
    { UwbApplicationConfigurationParameterType::SlotDuration, [](const UwbConfiguration& config, DeviceType deviceType) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetSlotDuration();
     } },
    { UwbApplicationConfigurationParameterType::RangingInterval, [](const UwbConfiguration& config, DeviceType deviceType) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetRangingInterval();
     } },
    { UwbApplicationConfigurationParameterType::MacFcsType, [](const UwbConfiguration& config, DeviceType deviceType) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetMacAddressFcsType();
     } },
    { UwbApplicationConfigurationParameterType::RFrameConfiguration, [](const UwbConfiguration& config, DeviceType deviceType) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetRFrameConfig();
     } },
    { UwbApplicationConfigurationParameterType::PreambleCodeIndex, [](const UwbConfiguration& config, DeviceType deviceType) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetPreambleCodeIndex();
     } },
    { UwbApplicationConfigurationParameterType::RangingTimeStruct, [](const UwbConfiguration& config, DeviceType deviceType) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetRangingTimeStruct();
     } },
    { UwbApplicationConfigurationParameterType::SlotsPerRangingRound, [](const UwbConfiguration& config, DeviceType deviceType) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetSlotsPerRangingRound();
     } },
    { UwbApplicationConfigurationParameterType::PrfMode, [](const UwbConfiguration& config, DeviceType deviceType) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetPrfMode();
     } },
    { UwbApplicationConfigurationParameterType::ScheduledMode, [](const UwbConfiguration& config, DeviceType deviceType) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetSchedulingMode();
     } },
    { UwbApplicationConfigurationParameterType::KeyRotationRate, [](const UwbConfiguration& config, DeviceType deviceType) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetKeyRotationRate();
     } },
    { UwbApplicationConfigurationParameterType::MacAddressMode, [](const UwbConfiguration& config, DeviceType deviceType) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetMacAddressMode();
     } },
    { UwbApplicationConfigurationParameterType::MaxRangingRoundRetry, [](const UwbConfiguration& config, DeviceType deviceType) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetMaxRangingRoundRetry();
     } },
    { UwbApplicationConfigurationParameterType::UwbInitiationTime, [](const UwbConfiguration& config, DeviceType deviceType) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetUwbInitiationTime();
     } },
    { UwbApplicationConfigurationParameterType::HoppingMode, [](const UwbConfiguration& config, DeviceType deviceType) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetHoppingMode();
     } },
    // {  UwbApplicationConfigurationParameterType::ResultReportConfig, []( const UwbConfiguration & config) -> std::optional< UwbApplicationConfigurationParameterValue> {
    //      return config.GetResultReportConfigurations();
    //  } },

    // params with different names
    // {  UwbApplicationConfigurationParameterType::RangingRoundUsage, []( const UwbConfiguration & config) -> std::optional< UwbApplicationConfigurationParameterValue> {
    //      return config.GetRangingMethod();
    //  } },
    { UwbApplicationConfigurationParameterType::DeviceMacAddress, [](const UwbConfiguration& config, DeviceType deviceType) -> std::optional<UwbApplicationConfigurationParameterValue> {
         auto mode = config.GetMacAddressMode();
         if (not mode) {
             return std::nullopt;
         }
         if (deviceType == DeviceType::Controller) {
             return config.GetControllerMacAddress();
         } else {
             if (mode == uwb::UwbMacAddressType::Short) {
                 return config.GetControleeShortMacAddress();
             } else {
                 // TODO what do we do here
                 return std::nullopt;
             }
         }
         return std::nullopt;
     } },
    { UwbApplicationConfigurationParameterType::DestinationMacAddresses, [](const UwbConfiguration& config, DeviceType deviceType) -> std::optional<UwbApplicationConfigurationParameterValue> {
         auto mode = config.GetMacAddressMode();
         if (not mode) {
             return std::nullopt;
         }
         if (deviceType == DeviceType::Controller) {
             if (mode == uwb::UwbMacAddressType::Short) {
                 return config.GetControleeShortMacAddress(); // TODO this should reflect the possibility for multiple peers
             } else {
                 // TODO what do we do here
                 return std::nullopt;
             }
         } else {
             return config.GetControllerMacAddress();
         }
     } },
    // TODO figure out the rest of the uci params
};

std::vector<UwbApplicationConfigurationParameter>
uwb::protocol::fira::GetUciConfigParams(const UwbConfiguration& uwbConfiguration, DeviceType deviceType)
{
    std::vector<UwbApplicationConfigurationParameter> result;
    for (const auto& [uciTag, generator] : uciGenerators) {
        const auto uciValue = generator(uwbConfiguration, deviceType);
        if (not uciValue) {
            continue;
        }
        result.push_back(UwbApplicationConfigurationParameter{
            .Type = uciTag,
            .Value = uciValue.value() });
    }
    return result;
}