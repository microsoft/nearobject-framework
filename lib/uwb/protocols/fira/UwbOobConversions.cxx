
#include <uwb/protocols/fira/UwbConfigurationBuilder.hxx>
#include <uwb/protocols/fira/UwbOobConversions.hxx>
#include <uwb/protocols/fira/UwbSessionData.hxx>

using namespace uwb::protocol::fira;
/**
 * @brief map of uci parameter type to a function of how to generate that uci parameter value given UwbConfiguration
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
    { UwbApplicationConfigurationParameterType::ResultReportConfig, [](const UwbConfiguration& config, DeviceType deviceType) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetResultReportConfigurations();
     } },

    // params with different names
    { UwbApplicationConfigurationParameterType::RangingRoundUsage, [](const UwbConfiguration& config, DeviceType deviceType) -> std::optional<UwbApplicationConfigurationParameterValue> {
         auto rangingMethod = config.GetRangingMethod();
         if (not rangingMethod) {
             return std::nullopt;
         }
         if (rangingMethod->Method == RangingDirection::SingleSidedTwoWay and rangingMethod->ReportMode == MeasurementReportMode::Deferred) {
             return RangingRoundUsage::SingleSidedTwoWayRangingWithDeferredMode;
         } else if (rangingMethod->Method == RangingDirection::DoubleSidedTwoWay and rangingMethod->ReportMode == MeasurementReportMode::Deferred) {
             return RangingRoundUsage::DoubleSidedTwoWayRangingWithDeferredMode;
         } else if (rangingMethod->Method == RangingDirection::SingleSidedTwoWay and rangingMethod->ReportMode == MeasurementReportMode::NonDeferred) {
             return RangingRoundUsage::SingleSidedTwoWayRangingNonDeferredMode;
         } else if (rangingMethod->Method == RangingDirection::DoubleSidedTwoWay and rangingMethod->ReportMode == MeasurementReportMode::NonDeferred) {
             return RangingRoundUsage::DoubleSidedTwoWayRangingNonDeferredMode;
         } else {
             return std::nullopt;
         }
     } },
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
    { UwbApplicationConfigurationParameterType::PrfMode, [](const UwbConfiguration& config, DeviceType deviceType) -> std::optional<UwbApplicationConfigurationParameterValue> {
         auto prfMode = config.GetPrfMode();
         if (prfMode == PrfMode::Bprf) {
             return PrfModeDetailed::Bprf62MHz;
         } else {                                // HPRF
             return PrfModeDetailed::Hprf124MHz; // TODO: Is there a way to determine OOB which Hprf frequency is used?
         }
     } },
    // TODO figure out the rest of the uci params
};

std::vector<UwbApplicationConfigurationParameter>
uwb::protocol::fira::GetUciConfigParams(const UwbConfiguration& uwbConfiguration, DeviceType deviceType)
{
    std::vector<UwbApplicationConfigurationParameter> result;
    for (const auto& [uciParameterType, generator] : uciGenerators) {
        const auto uciValue = generator(uwbConfiguration, deviceType);
        if (not uciValue) {
            continue;
        }
        result.push_back(UwbApplicationConfigurationParameter{
            .Type = uciParameterType,
            .Value = uciValue.value() });
    }
    return result;
}

UwbSessionData
uwb::protocol::fira::GetUwbSessionData(std::vector<UwbApplicationConfigurationParameter> applicationConfigurationParameters)
{
    UwbSessionData sessionData{};
    UwbConfiguration::Builder builder{};

    DeviceType deviceType;
    UwbMacAddress deviceMacAddress;
    std::unordered_set<UwbMacAddress> destinationMacAddresses;

    for (const auto& applicationConfigurationParameter : applicationConfigurationParameters) {
        const auto& parameterType = applicationConfigurationParameter.Type;
        const auto& parameterValue = applicationConfigurationParameter.Value;

        switch (parameterType) {
        case UwbApplicationConfigurationParameterType::DeviceType:
            deviceType = std::get<DeviceType>(parameterValue);
            break;
        case UwbApplicationConfigurationParameterType::RangingRoundUsage:
            // TODO
            break;
        case UwbApplicationConfigurationParameterType::StsConfiguration:
            builder.SetStsConfiguration(std::get<StsConfiguration>(parameterValue));
            break;
        case UwbApplicationConfigurationParameterType::MultiNodeMode:
            builder.SetMultiNodeMode(std::get<MultiNodeMode>(parameterValue));
            break;
        case UwbApplicationConfigurationParameterType::ChannelNumber:
            builder.SetChannel(std::get<Channel>(parameterValue));
            break;
        case UwbApplicationConfigurationParameterType::NumberOfControlees:
            // TODO
            break;
        case UwbApplicationConfigurationParameterType::DeviceMacAddress:
            // In case this parameter comes before DeviceType, store and set correct parameter later
            deviceMacAddress = std::get<UwbMacAddress>(parameterValue);
            builder.SetMacAddressController(deviceMacAddress);
            break;
        case UwbApplicationConfigurationParameterType::DestinationMacAddresses:
            // In case this parameter comes before DeviceType, store and set correct parameter later
            destinationMacAddresses = std::get<std::unordered_set<UwbMacAddress>>(parameterValue);
            // TODO: Do this correctly
            builder.SetMacAddressControleeShort(*destinationMacAddresses.begin());
            break;
        case UwbApplicationConfigurationParameterType::SlotDuration:
            builder.SetSlotDuration(std::get<uint16_t>(parameterValue));
            break;
        case UwbApplicationConfigurationParameterType::RangingInterval:
            builder.SetRangingInterval(std::get<uint32_t>(parameterValue));
            break;
        case UwbApplicationConfigurationParameterType::StsIndex:
            // TODO
            break;
        case UwbApplicationConfigurationParameterType::MacFcsType:
            builder.SetMacAddressFcsType(std::get<UwbMacAddressFcsType>(parameterValue));
            break;
        case UwbApplicationConfigurationParameterType::RangingRoundControl:
            // TODO
            break;
        case UwbApplicationConfigurationParameterType::AoAResultRequest:
            // TODO
            break;
        case UwbApplicationConfigurationParameterType::RangeDataNotificationConfig:
            // TODO
            break;
        case UwbApplicationConfigurationParameterType::RangeDataNotificationProximityNear:
            // TODO
            break;
        case UwbApplicationConfigurationParameterType::RangeDataNotificationProximityFar:
            // TODO
            break;
        case UwbApplicationConfigurationParameterType::DeviceRole:
            builder.SetDeviceRole(std::get<DeviceRole>(parameterValue));
            break;
        case UwbApplicationConfigurationParameterType::RFrameConfiguration:
            builder.SetStsPacketConfiguration(std::get<RFrameConfiguration>(parameterValue));
            break;
        case UwbApplicationConfigurationParameterType::PreambleCodeIndex:
            builder.SetPreambleCodeIndex(std::get<uint8_t>(parameterValue));
            break;
        case UwbApplicationConfigurationParameterType::SfdId:
            // TODO
            break;
        case UwbApplicationConfigurationParameterType::PsduDataRate:
            // TODO
            break;
        case UwbApplicationConfigurationParameterType::PreambleDuration:
            // TODO
            break;
        case UwbApplicationConfigurationParameterType::RangingTimeStruct:
            builder.SetRangingTimeStruct(std::get<RangingMode>(parameterValue));
            break;
        case UwbApplicationConfigurationParameterType::SlotsPerRangingRound:
            builder.SetMaxSlotsPerRangingRound(std::get<uint8_t>(parameterValue));
            break;
        case UwbApplicationConfigurationParameterType::TxAdaptivePayloadPower:
            // TODO
            break;
        case UwbApplicationConfigurationParameterType::ResponderSlotIndex:
            // TODO
            break;
        case UwbApplicationConfigurationParameterType::PrfMode:
            if (std::get<PrfModeDetailed>(parameterValue) == PrfModeDetailed::Bprf62MHz) {
                builder.SetPrfMode(PrfMode::Bprf);
            } else { // PrfModeDetailed::Hprf124MHz || PrfModeDetailed::Hprf249MHz
                builder.SetPrfMode(PrfMode::Hprf);
            }
            break;
        case UwbApplicationConfigurationParameterType::ScheduledMode:
            builder.SetSchedulingMode(std::get<SchedulingMode>(parameterValue));
            break;
        case UwbApplicationConfigurationParameterType::KeyRotation:
            // TODO
            break;
        case UwbApplicationConfigurationParameterType::KeyRotationRate:
            builder.SetKeyRotationRate(std::get<uint8_t>(parameterValue));
            break;
        case UwbApplicationConfigurationParameterType::SessionPriority:
            // TODO
            break;
        case UwbApplicationConfigurationParameterType::MacAddressMode:
            builder.SetMacAddressType(std::get<UwbMacAddressType>(parameterValue));
            break;
        case UwbApplicationConfigurationParameterType::VendorId:
            sessionData.staticRangingInfo->VendorId = std::get<uint16_t>(parameterValue);
            break;
        case UwbApplicationConfigurationParameterType::StaticStsIv:
            sessionData.staticRangingInfo->InitializationVector = std::get<StaticStsInitializationVector>(parameterValue);
            break;
        case UwbApplicationConfigurationParameterType::NumberOfStsSegments:
            // TODO
            break;
        case UwbApplicationConfigurationParameterType::MaxRangingRoundRetry:
            builder.SetMaxRangingRoundRetry(std::get<uint16_t>(parameterValue));
            break;
        case UwbApplicationConfigurationParameterType::UwbInitiationTime:
            builder.SetUwbInitiationTime(std::get<uint32_t>(parameterValue));
            break;
        case UwbApplicationConfigurationParameterType::HoppingMode:
            // TODO
            break;
        case UwbApplicationConfigurationParameterType::BlockStrideLength:
            // TODO
            break;
        case UwbApplicationConfigurationParameterType::ResultReportConfig: {
            auto resultReportConfigurations = std::get<std::unordered_set<ResultReportConfiguration>>(parameterValue);
            for (const auto& resultReportConfiguration : resultReportConfigurations) {
                builder.AddResultReportConfiguration(resultReportConfiguration);
            }
            break;
        }
        case UwbApplicationConfigurationParameterType::InBandTerminationAttemptCount:
            // TODO
            break;
        case UwbApplicationConfigurationParameterType::SubSessionId:
            sessionData.subSessionId = std::get<uint32_t>(parameterValue);
            break;
        case UwbApplicationConfigurationParameterType::BprfPhrDataRate:
            // TODO
            break;
        case UwbApplicationConfigurationParameterType::MaxNumberOfMeasurements:
            // TODO
            break;
        case UwbApplicationConfigurationParameterType::StsLength:
            // TODO
            break;
        default:
            // TODO: Log error
            break;
        }
    }

    // TODO: This assumes short mac addresses and only 1 address in the destination set.
    //       Need to figure out how to do this correctly
    if (deviceType == DeviceType::Controller) {
        builder.SetMacAddressController(deviceMacAddress);
        builder.SetMacAddressControleeShort(*destinationMacAddresses.begin());
    } else { // DeviceType::Controlee
        builder.SetMacAddressController(*destinationMacAddresses.begin());
        builder.SetMacAddressControleeShort(deviceMacAddress);
    }

    sessionData.uwbConfiguration = std::move(builder);
    sessionData.uwbConfigurationAvailable = true;

    return sessionData;
}
