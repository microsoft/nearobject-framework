
#include <nearobject/cli/NearObjectCliData.hxx>
#include <uwb/protocols/fira/UwbConfigurationBuilder.hxx>

using namespace nearobject::cli;
using uwb::protocol::fira::UwbConfiguration;

UwbConfigurationData::operator UwbConfiguration() const noexcept
{
    UwbConfiguration::Builder builder{};

    if (firaPhyVersion.has_value()) {
        builder.SetFiraVersionPhy(firaMacVersion.value());
    }
    if (firaMacVersion.has_value()) {
        builder.SetFiraVersionMac(firaMacVersion.value());
    }
    if (deviceRole.has_value()) {
        builder.SetDeviceRole(deviceRole.value());
    }
    if (rangingDirection.has_value() && rangingMeasurementReportMode.has_value()) {
        builder.SetRangingMethod({ rangingDirection.value(), rangingMeasurementReportMode.value()});
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
    if (preableCodeIndex.has_value()) {
        builder.SetPreableCodeIndex(preableCodeIndex.value());
    }
    for (const auto& resultReportConfiguration : resultReportConfigurations) {
        builder.AddResultReportConfiguration(resultReportConfiguration);
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
