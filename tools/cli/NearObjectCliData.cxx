
#include <nearobject/cli/NearObjectCliData.hxx>
#include <uwb/protocols/fira/UwbConfigurationBuilder.hxx>

using namespace nearobject::cli;
using uwb::protocol::fira::UwbApplicationConfigurationParameterType;
using uwb::protocol::fira::UwbApplicationConfigurationParameterValue;
using uwb::protocol::fira::UwbConfiguration;

UwbConfigurationData::operator UwbConfiguration() const noexcept
{
    UwbConfiguration::Builder builder{};

    // Handle inputs that require parsing from string type.
    if (!firaPhyVersionString.empty()) {
        auto firaPhyVersion = uwb::protocol::fira::StringToVersion(firaPhyVersionString);
        if (firaPhyVersion.has_value()) {
            builder.SetFiraVersionPhy(firaPhyVersion.value());
        }
    }
    if (!firaMacVersionString.empty()) {
        auto firaMacVersion = uwb::protocol::fira::StringToVersion(firaMacVersionString);
        if (firaMacVersion.has_value()) {
            builder.SetFiraVersionMac(firaMacVersion.value());
        }
    }
    if (!resultReportConfigurationString.empty()) {
        auto resultReportConfigurations = uwb::protocol::fira::StringToResultReportConfiguration(resultReportConfigurationString);
        for (const auto& resultReportConfiguration : resultReportConfigurations.value_or(std::unordered_set<uwb::protocol::fira::ResultReportConfiguration>{})) {
            builder.AddResultReportConfiguration(resultReportConfiguration);
        }
    }

    // Handle other inputs which have already been parsed.
    if (deviceRole.has_value()) {
        builder.SetDeviceRole(deviceRole.value());
    }
    if (rangingDirection.has_value() && rangingMeasurementReportMode.has_value()) {
        builder.SetRangingMethod({ rangingDirection.value(), rangingMeasurementReportMode.value() });
    }
    if (stsConfiguration.has_value()) {
        builder.SetStsConfiguration(stsConfiguration.value());
    }
    if (multiNodeMode.has_value()) {
        builder.SetMultiNodeMode(multiNodeMode.value());
    }
    if (rangingTimeStruct.has_value()) {
        builder.SetRangingTimeStruct(rangingTimeStruct.value());
    }
    if (schedulingMode.has_value()) {
        builder.SetSchedulingMode(schedulingMode.value());
    }
    if (hoppingMode.has_value()) {
        builder.SetHoppingMode(hoppingMode.value());
    }
    if (blockStriding.has_value()) {
        builder.SetBlockStriding(blockStriding.value());
    }
    if (uwbInitiationTime.has_value()) {
        builder.SetUwbInitiationTime(uwbInitiationTime.value());
    }
    if (channel.has_value()) {
        builder.SetChannel(channel.value());
    }
    if (rframeConfig.has_value()) {
        builder.SetStsPacketConfiguration(rframeConfig.value());
    }
    if (convolutionalCodeConstraintLength.has_value()) {
        builder.SetConvolutionalCodeConstraintLength(convolutionalCodeConstraintLength.value());
    }
    if (prfMode.has_value()) {
        builder.SetPrfMode(prfMode.value());
    }
    if (sp0PhySetNumber.has_value()) {
        builder.SetSp0PhySetNumber(sp0PhySetNumber.value());
    }
    if (sp1PhySetNumber.has_value()) {
        builder.SetSp1PhySetNumber(sp1PhySetNumber.value());
    }
    if (sp3PhySetNumber.has_value()) {
        builder.SetSp3PhySetNumber(sp3PhySetNumber.value());
    }
    if (preambleCodeIndex.has_value()) {
        builder.SetPreambleCodeIndex(preambleCodeIndex.value());
    }
    if (macAddressMode.has_value()) {
        builder.SetMacAddressType(macAddressMode.value());
    }
    if (controleeShortMacAddress.has_value()) {
        builder.SetMacAddressControleeShort(controleeShortMacAddress.value());
    }
    if (controllerMacAddress.has_value()) {
        builder.SetMacAddressController(controllerMacAddress.value());
    }
    if (slotsPerRangingRound.has_value()) {
        builder.SetMaxSlotsPerRangingRound(slotsPerRangingRound.value());
    }
    if (maxContentionPhaseLength.has_value()) {
        builder.SetMaxContentionPhaseLength(maxContentionPhaseLength.value());
    }
    if (slotDuration.has_value()) {
        builder.SetSlotDuration(slotDuration.value());
    }
    if (rangingInterval.has_value()) {
        builder.SetRangingInterval(rangingInterval.value());
    }
    if (keyRotationRate.has_value()) {
        builder.SetKeyRotationRate(keyRotationRate.value());
    }
    if (macAddressFcsType.has_value()) {
        builder.SetMacAddressFcsType(macAddressFcsType.value());
    }
    if (maxRangingRoundRetry.has_value()) {
        builder.SetMaxRangingRoundRetry(maxRangingRoundRetry.value());
    }

    return builder;
}

std::unordered_map<UwbApplicationConfigurationParameterType, UwbApplicationConfigurationParameterValue>
UwbApplicationConfigurationParameterData::GetValueMap() const
{
    std::unordered_map<UwbApplicationConfigurationParameterType, UwbApplicationConfigurationParameterValue> valuesMap;
    if (deviceRole.has_value()) {
        valuesMap[UwbApplicationConfigurationParameterType::DeviceRole] = deviceRole.value();
    }
    if (multiNodeMode.has_value()) {
        valuesMap[UwbApplicationConfigurationParameterType::MultiNodeMode] = multiNodeMode.value();
    }
    if (numberOfControlees.has_value()) {
        valuesMap[UwbApplicationConfigurationParameterType::NumberOfControlees] = numberOfControlees.value();
    }
    if (deviceMacAddress.has_value()) {
        valuesMap[UwbApplicationConfigurationParameterType::DeviceMacAddress] = deviceMacAddress.value();
    }
    if (destinationMacAddresses.has_value()) {
        valuesMap[UwbApplicationConfigurationParameterType::DestinationMacAddresses] = destinationMacAddresses.value();
    }
    if (deviceType.has_value()) {
        valuesMap[UwbApplicationConfigurationParameterType::DeviceType] = deviceType.value();
    }
    return valuesMap;
}
