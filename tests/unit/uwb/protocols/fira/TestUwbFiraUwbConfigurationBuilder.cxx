
#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <initializer_list>
#include <unordered_map>
#include <unordered_set>

#include <uwb/protocols/fira/UwbConfiguration.hxx>
#include <uwb/protocols/fira/UwbConfigurationBuilder.hxx>

namespace uwb::protocol::fira::test
{
constexpr uint16_t FiraVersionMacValue{ 0xABCDU };
constexpr uint16_t FiraVersionPhyValue{ 0xDCBAU };
constexpr Channel ChannelValue{ uwb::protocol::fira::Channel::C10 };
constexpr DeviceRole DeviceRoleValue{ DeviceRole::Initiator };
constexpr RangingMethod RangingMethodValue{ RangingDirection::SingleSidedTwoWay, MeasurementReportMode::Deferred };
constexpr StsConfiguration StsConfigurationValue{ StsConfiguration::Static };
constexpr MultiNodeMode MultiNodeModeValue{ MultiNodeMode::Unicast };
constexpr RangingMode RangingTimeStructValue{ RangingMode::Block };
constexpr SchedulingMode SchedulingModeValue{ SchedulingMode::Time };
constexpr uint32_t UwbInitiationTimeValue{ 0xAABBCCDD };
constexpr StsPacketConfiguration StsPacketConfigurationValue{ StsPacketConfiguration::SP0 };
constexpr ConvolutionalCodeConstraintLength ConvolutionalCodeConstraintLengthValue{ ConvolutionalCodeConstraintLength::K3 };
constexpr PrfMode PrfModeValue{ PrfMode::Bprf };
constexpr uint8_t Sp0Value{ 10 };
constexpr uint8_t Sp1Value{ 20 };
constexpr uint8_t Sp3Value{ 30 };
constexpr uint8_t PreableCodeIndexValue{ 25 };
constexpr uwb::UwbMacAddressType UwbMacAddressTypeValue{ uwb::UwbMacAddressType::Extended };
constexpr uwb::UwbMacAddressFcsType UwbMacAddressFcsTypeValue{ uwb::UwbMacAddressFcsType::Crc32 };
const uwb::UwbMacAddress UwbMacAddressControleeShortValue{ uwb::UwbMacAddress{ std::array<uint8_t, 2>{ 0x00, 0x01 } } };
const uwb::UwbMacAddress UwbMacAddressControllerValue{ uwb::UwbMacAddress{ std::array<uint8_t, 8>{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 } } };
constexpr uint8_t SlotsPerRangingRoundValue{ 0 };
constexpr uint16_t SlotDurationValue{ 255 };
constexpr uint32_t RangingIntervalValue{ 0x1234 };
constexpr uint8_t KeyRotationRateValue{ 100 };
constexpr ResultReportConfiguration ResultReportConfigurationValue1{ ResultReportConfiguration::AoAAzimuthReport };
constexpr ResultReportConfiguration ResultReportConfigurationValue2{ ResultReportConfiguration::AoAElevationReport };
constexpr ResultReportConfiguration ResultReportConfigurationValue3{ ResultReportConfiguration::AoAFoMReport };
constexpr ResultReportConfiguration ResultReportConfigurationValue4{ ResultReportConfiguration::TofReport };
constexpr uint8_t MaxContentionPhaseLengthValue{ 9 };
constexpr uint16_t MaxRangingRoundRetryValue{ 100 };
constexpr bool HoppingModeValue{ true };
constexpr bool BlockStridingValue{ true };
} // namespace uwb::protocol::fira::test

TEST_CASE("uwb configuration objects can be created with builder", "[basic]")
{
    using namespace uwb::protocol::fira;

    SECTION("created object reflects values provided to builder")
    {
        // clang-format off
        UwbConfiguration uwbConfiguration = UwbConfiguration::Create()
            .OnChannel(test::ChannelValue)
            .With()
                .FiraVersion()
                    .Mac(test::FiraVersionMacValue)
                    .Phy(test::FiraVersionPhyValue)
                .DeviceRole(test::DeviceRoleValue)
                .RangingMethod(test::RangingMethodValue)
                .StsConfiguration(test::StsConfigurationValue)
                .MultiNodeMode(test::MultiNodeModeValue)
                .RangingTimeStruct(test::RangingTimeStructValue)
                .SchedulingMode(test::SchedulingModeValue)
                .UwbInitiationTime(test::UwbInitiationTimeValue)
                .StsPacketConfiguration(test::StsPacketConfigurationValue)
                .ConvolutionalCodeConstraintLength(test::ConvolutionalCodeConstraintLengthValue)
                .PrfMode(test::PrfModeValue)
                .PhySetNumber()
                    .Sp0(test::Sp0Value)
                    .Sp1(test::Sp1Value)
                    .Sp3(test::Sp3Value)
                .PreableCodeIndex(test::PreableCodeIndexValue)
                .MacAddress()
                    .Type(test::UwbMacAddressTypeValue)
                    .FcsType(test::UwbMacAddressFcsTypeValue)
                    .OfControleeShort(test::UwbMacAddressControleeShortValue)
                    .OfController(test::UwbMacAddressControllerValue)
                .SlotsPerRangingRound(test::SlotsPerRangingRoundValue)
                .SlotDuration(test::SlotDurationValue)
                .RangingInterval(test::RangingIntervalValue)
                .KeyRotationRate(test::KeyRotationRateValue)
            .Supports()
                .ResultReportConfiguration(test::ResultReportConfigurationValue1)
                .ResultReportConfiguration(test::ResultReportConfigurationValue2)
                .ResultReportConfiguration(test::ResultReportConfigurationValue3)
                .ResultReportConfiguration(test::ResultReportConfigurationValue4)
            .Maximum()
                .ContentionPhaseLength(test::MaxContentionPhaseLengthValue)
                .RangingRoundRetry(test::MaxRangingRoundRetryValue)
            .SetHoppingMode(test::HoppingModeValue)
            .SetBlockStriding(test::BlockStridingValue)
                ;
        // clang-format on

        REQUIRE(uwbConfiguration.GetChannel() == test::ChannelValue);
        REQUIRE(uwbConfiguration.GetFiraMacVersion() == test::FiraVersionMacValue);
        REQUIRE(uwbConfiguration.GetFiraPhyVersion() == test::FiraVersionPhyValue);
        REQUIRE(uwbConfiguration.GetDeviceRole() == test::DeviceRoleValue);
        REQUIRE(uwbConfiguration.GetRangingMethod() == test::RangingMethodValue);
        REQUIRE(uwbConfiguration.GetStsConfiguration() == test::StsConfigurationValue);
        REQUIRE(uwbConfiguration.GetMultiNodeMode() == test::MultiNodeModeValue);
        REQUIRE(uwbConfiguration.GetRangingTimeStruct() == test::RangingTimeStructValue);
        REQUIRE(uwbConfiguration.GetSchedulingMode() == test::SchedulingModeValue);
        REQUIRE(uwbConfiguration.GetUwbInitiationTime() == test::UwbInitiationTimeValue);
        REQUIRE(uwbConfiguration.GetRFrameConfig() == test::StsPacketConfigurationValue);
        REQUIRE(uwbConfiguration.GetConvolutionalCodeConstraintLength() == test::ConvolutionalCodeConstraintLengthValue);
        REQUIRE(uwbConfiguration.GetPrfMode() == test::PrfModeValue);
        REQUIRE(uwbConfiguration.GetSp0PhySetNumber() == test::Sp0Value);
        REQUIRE(uwbConfiguration.GetSp1PhySetNumber() == test::Sp1Value);
        REQUIRE(uwbConfiguration.GetSp3PhySetNumber() == test::Sp3Value);
        REQUIRE(uwbConfiguration.GetPreambleCodeIndex() == test::PreableCodeIndexValue);
        REQUIRE(uwbConfiguration.GetMacAddressMode() == test::UwbMacAddressTypeValue);
        REQUIRE(uwbConfiguration.GetMacAddressFcsType() == test::UwbMacAddressFcsTypeValue);
        REQUIRE(uwbConfiguration.GetControleeShortMacAddress() == test::UwbMacAddressControleeShortValue);
        REQUIRE(uwbConfiguration.GetControllerMacAddress() == test::UwbMacAddressControllerValue);
        REQUIRE(uwbConfiguration.GetSlotsPerRangingRound() == test::SlotsPerRangingRoundValue);
        REQUIRE(uwbConfiguration.GetSlotDuration() == test::SlotDurationValue);
        REQUIRE(uwbConfiguration.GetRangingInterval() == test::RangingIntervalValue);
        REQUIRE(uwbConfiguration.GetKeyRotationRate() == test::KeyRotationRateValue);
        REQUIRE(uwbConfiguration.GetMaxContentionPhaseLength() == test::MaxContentionPhaseLengthValue);
        REQUIRE(uwbConfiguration.GetMaxRangingRoundRetry() == test::MaxRangingRoundRetryValue);
        REQUIRE(uwbConfiguration.GetHoppingMode() == test::HoppingModeValue);
        REQUIRE(uwbConfiguration.GetBlockStriding() == test::BlockStridingValue);

        const auto resultReportConfigurations = uwbConfiguration.GetResultReportConfigurations();
        REQUIRE(resultReportConfigurations.size() == 4);
        REQUIRE(resultReportConfigurations.contains(test::ResultReportConfigurationValue1));
        REQUIRE(resultReportConfigurations.contains(test::ResultReportConfigurationValue2));
        REQUIRE(resultReportConfigurations.contains(test::ResultReportConfigurationValue3));
        REQUIRE(resultReportConfigurations.contains(test::ResultReportConfigurationValue4));
    }
}
