
#include <catch2/catch_test_macros.hpp>

#include <notstd/utility.hxx>

#include <algorithm>
#include <array>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <memory>

#include <TlvBer.hxx>
#include <uwb/protocols/fira/UwbCapability.hxx>

using namespace uwb::protocol::fira;
using encoding::TlvBer;

bool
tagIsPresent(UwbCapability::ParameterTag tag, const encoding::TlvBer& tlv)
{
    return std::ranges::any_of(tlv.GetValues(), [&tag](const auto& subTlv) {
        const auto subTlvTag = subTlv.GetTag();
        return (subTlvTag.size() == 1) && (subTlvTag[0] == notstd::to_underlying(tag));
    });
}

struct TestUwbCapability
{
    static constexpr auto MultiNodeModesDefault = {
        MultiNodeMode::OneToMany,
    };

    static constexpr auto DeviceRolesDefault = {
        DeviceRole::Initiator,
    };

    static constexpr auto StsConfigurationsDefault = {
        StsConfiguration::Static,
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
    };

    static constexpr auto SchedulingModeTypesDefault = {
        SchedulingMode::Time,
    };

    static constexpr auto RangingTimeStructsDefault = {
        RangingMode::Block,
    };

    static constexpr auto RangingConfigurationsDefault = {
        RangingConfiguration{ RangingMethod::OneWay, MeasurementReportMode::None },
        RangingConfiguration{ RangingMethod::SingleSidedTwoWay, MeasurementReportMode::Deferred },
    };

    static constexpr auto ConvolutionalCodeConstraintLengthsDefault = {
        ConvolutionalCodeConstraintLength::K3,
        ConvolutionalCodeConstraintLength::K7,
    };

    static constexpr auto ChannelsDefault = {
        Channel::C5,
        Channel::C6,
        Channel::C8,
        Channel::C9
    };

    static constexpr auto BprfParameterSetsDefault = {
        BprfParameter::Set1,
        BprfParameter::Set2,
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
        HprfParameter::Set35,
    };

    static constexpr uint32_t phyVersionDefault = 0xAABBCCDD;
    static constexpr uint32_t macVersionDefault = 0xAABBCCEE;

    uint32_t FiraPhyVersionRange{ phyVersionDefault };
    uint32_t FiraMacVersionRange{ macVersionDefault };
    static const bool ExtendedMacAddressDefault{ false };
    static const bool UwbInitiationTimeDefault{ false };
    static const bool AngleOfArrivalFomDefault{ false };
    static const bool BlockStridingDefault{ true };
    static const bool HoppingModeDefault{ true };

    static const UwbCapability testUwbCapability;

    static const std::vector<uint8_t> phyRangeExpected;
    static const std::vector<uint8_t> macRangeExpected;
    static const std::vector<uint8_t> ExtendedMacAddressExpected;
    static const std::vector<uint8_t> UwbInitiationTimeExpected;
    static const std::vector<uint8_t> AngleOfArrivalFomExpected;
    static const std::vector<uint8_t> BlockStridingExpected;
    static const std::vector<uint8_t> HoppingModeExpected;
    static const std::vector<uint8_t> MultiNodeModesExpected;
    static const std::vector<uint8_t> DeviceRolesExpected;
    static const std::vector<uint8_t> StsConfigurationsExpected;
    static const std::vector<uint8_t> RFrameConfigurationsExpected;
    static const std::vector<uint8_t> AngleOfArrivalTypesExpected;
    static const std::vector<uint8_t> AoaSupportExpected;
    static const std::vector<uint8_t> SchedulingModeTypesExpected;
    static const std::vector<uint8_t> RangingTimeStructsExpected;
    static const std::vector<uint8_t> RangingConfigurationsExpected;
    static const std::vector<uint8_t> ConvolutionalCodeConstraintLengthsExpected;
    static const std::vector<uint8_t> ChannelsExpected;
    static const std::vector<uint8_t> BprfParameterSetsExpected;
    static const std::vector<uint8_t> HprfParameterSetsExpected;

    static const std::vector<std::tuple<UwbCapability::ParameterTag, std::vector<uint8_t>>>
        tagAndExpected;
};

// TODO this test code is reliant on the bitmaps staying the same
const std::vector<uint8_t> TestUwbCapability::phyRangeExpected{ 0xAA, 0xBB, 0xCC, 0xDD };
const std::vector<uint8_t> TestUwbCapability::macRangeExpected{ 0xAA, 0xBB, 0xCC, 0xEE };
const std::vector<uint8_t> TestUwbCapability::ExtendedMacAddressExpected{ 0 };
const std::vector<uint8_t> TestUwbCapability::UwbInitiationTimeExpected{ 0 };
const std::vector<uint8_t> TestUwbCapability::AngleOfArrivalFomExpected{ 0 };
const std::vector<uint8_t> TestUwbCapability::BlockStridingExpected{ 1 };
const std::vector<uint8_t> TestUwbCapability::HoppingModeExpected{ 1 };
const std::vector<uint8_t> TestUwbCapability::MultiNodeModesExpected{ 0x02 };
const std::vector<uint8_t> TestUwbCapability::DeviceRolesExpected{ 0x2 };
const std::vector<uint8_t> TestUwbCapability::StsConfigurationsExpected{ 0x5 };
const std::vector<uint8_t> TestUwbCapability::RFrameConfigurationsExpected{ 0xA };
const std::vector<uint8_t> TestUwbCapability::AngleOfArrivalTypesExpected{ 0x3 };
const std::vector<uint8_t> TestUwbCapability::AoaSupportExpected{ 0x3 };
const std::vector<uint8_t> TestUwbCapability::SchedulingModeTypesExpected{ 0x2 };
const std::vector<uint8_t> TestUwbCapability::RangingTimeStructsExpected{ 0x1 };
const std::vector<uint8_t> TestUwbCapability::RangingConfigurationsExpected{ 0x3 };
const std::vector<uint8_t> TestUwbCapability::ConvolutionalCodeConstraintLengthsExpected{ 0x3 };
const std::vector<uint8_t> TestUwbCapability::ChannelsExpected{ 0xF };
const std::vector<uint8_t> TestUwbCapability::BprfParameterSetsExpected{ 0x33 };
const std::vector<uint8_t> TestUwbCapability::HprfParameterSetsExpected{ 0x4, 0, 0, 0, 0x7F };

const std::vector<std::tuple<UwbCapability::ParameterTag, std::vector<uint8_t>>>
    TestUwbCapability::tagAndExpected = {
        std::make_tuple(UwbCapability::ParameterTag::FiraPhyVersionRange, TestUwbCapability::phyRangeExpected),
        std::make_tuple(UwbCapability::ParameterTag::FiraMacVersionRange, TestUwbCapability::macRangeExpected),
        std::make_tuple(UwbCapability::ParameterTag::DeviceRoles, TestUwbCapability::DeviceRolesExpected),
        std::make_tuple(UwbCapability::ParameterTag::RangingMethod, TestUwbCapability::RangingConfigurationsExpected),
        std::make_tuple(UwbCapability::ParameterTag::StsConfig, TestUwbCapability::StsConfigurationsExpected),
        std::make_tuple(UwbCapability::ParameterTag::MultiNodeMode, TestUwbCapability::MultiNodeModesExpected),
        std::make_tuple(UwbCapability::ParameterTag::RangingMode, TestUwbCapability::RangingTimeStructsExpected),
        std::make_tuple(UwbCapability::ParameterTag::ScheduledMode, TestUwbCapability::SchedulingModeTypesExpected),
        std::make_tuple(UwbCapability::ParameterTag::HoppingMode, TestUwbCapability::HoppingModeExpected),
        std::make_tuple(UwbCapability::ParameterTag::BlockStriding, TestUwbCapability::BlockStridingExpected),
        std::make_tuple(UwbCapability::ParameterTag::UwbInitiationTime, TestUwbCapability::UwbInitiationTimeExpected),
        std::make_tuple(UwbCapability::ParameterTag::Channels, TestUwbCapability::ChannelsExpected),
        std::make_tuple(UwbCapability::ParameterTag::RFrameConfig, TestUwbCapability::RFrameConfigurationsExpected),
        std::make_tuple(UwbCapability::ParameterTag::CcConstraintLength, TestUwbCapability::ConvolutionalCodeConstraintLengthsExpected),
        std::make_tuple(UwbCapability::ParameterTag::BprfParameterSets, TestUwbCapability::BprfParameterSetsExpected),
        std::make_tuple(UwbCapability::ParameterTag::HprfParameterSets, TestUwbCapability::HprfParameterSetsExpected),
        std::make_tuple(UwbCapability::ParameterTag::AoaSupport, TestUwbCapability::AoaSupportExpected),
        std::make_tuple(UwbCapability::ParameterTag::ExtendedMacAddress, TestUwbCapability::ExtendedMacAddressExpected),
    };

const UwbCapability
    TestUwbCapability::testUwbCapability = {
        TestUwbCapability::phyVersionDefault,
        TestUwbCapability::macVersionDefault,
        TestUwbCapability::ExtendedMacAddressDefault,
        TestUwbCapability::UwbInitiationTimeDefault,
        TestUwbCapability::AngleOfArrivalFomDefault,
        TestUwbCapability::BlockStridingDefault,
        TestUwbCapability::HoppingModeDefault,
        TestUwbCapability::MultiNodeModesDefault,
        TestUwbCapability::DeviceRolesDefault,
        TestUwbCapability::StsConfigurationsDefault,
        TestUwbCapability::RFrameConfigurationsDefault,
        TestUwbCapability::AngleOfArrivalTypesDefault,
        TestUwbCapability::SchedulingModeTypesDefault,
        TestUwbCapability::RangingTimeStructsDefault,
        TestUwbCapability::RangingConfigurationsDefault,
        TestUwbCapability::ConvolutionalCodeConstraintLengthsDefault,
        TestUwbCapability::ChannelsDefault,
        TestUwbCapability::BprfParameterSetsDefault,
        TestUwbCapability::HprfParameterSetsDefault
    };

TEST_CASE("Encoding into a TlvBer", "[basic]")
{
    // Convert a default-constructed UwbCapability object to an OOB data object (tlv).
    auto tlv = TestUwbCapability::testUwbCapability.ToOobDataObject();

    // Validate tag is as expected.
    const auto tag = tlv->GetTag();
    REQUIRE(tag.size() == 1);
    REQUIRE(tag[0] == UwbCapability::Tag);

    // Validate type is constructed.
    REQUIRE(tlv->IsConstructed());

    // Validate values are as expected.
    auto values = tlv->GetValues();
    REQUIRE(values.size() == TestUwbCapability::tagAndExpected.size());

    for (auto [tag, expected] : TestUwbCapability::tagAndExpected) {
        // verify that the tag exists in the values
        bool wefoundit = false;
        for (auto subtlv : values) {
            wefoundit = (subtlv.GetTag().size() == 1) and (subtlv.GetTag()[0] == notstd::to_underlying(tag));
            if (wefoundit) {
                // validate the value
                auto tlvValue = subtlv.GetValue();
                REQUIRE(std::equal(std::cbegin(expected), std::cend(expected), std::cbegin(tlvValue), std::cend(tlvValue)));
                break;
            }
        }
        if (not wefoundit) {
            REQUIRE(false);
        }
    }
}

// TODO find better place for this
template <class T>
bool
leftIsSubset(const std::vector<T>& lhs, const std::vector<T>& rhs)
{
    return std::ranges::any_of(lhs, [&rhs](auto elem) {
        return std::ranges::any_of(rhs, [elem](auto elem2) {
            return elem == elem2;
        });
    });
}

template <class T>
bool
leftUnorderedEquals(const std::vector<T>& lhs, const std::vector<T>& rhs)
{
    return leftIsSubset(lhs, rhs) and leftIsSubset(rhs, lhs);
}

TEST_CASE("Parsing from TlvBer", "[basic][protocol]")
{
    SECTION("FromOobDataObject correctly throws error with an invalid TlvBer tag")
    {
        auto tlv = TestUwbCapability::testUwbCapability.ToOobDataObject();

        encoding::TlvBer::Builder builder;
        auto invalidTlv = builder.SetAsCopyOfTlv(*tlv)
                              .SetTag(0xFF)
                              .Build();
        REQUIRE_THROWS(UwbCapability::FromOobDataObject(invalidTlv));
    }
    SECTION("FromOobDataObject works")
    {
        auto tlv = TestUwbCapability::testUwbCapability.ToOobDataObject();
        REQUIRE(tlv);
        UwbCapability decodedCapability;
        REQUIRE_NOTHROW(decodedCapability = UwbCapability::FromOobDataObject(*tlv));
        REQUIRE(decodedCapability.FiraPhyVersionRange == TestUwbCapability::testUwbCapability.FiraPhyVersionRange);
        REQUIRE(decodedCapability.FiraMacVersionRange == TestUwbCapability::testUwbCapability.FiraMacVersionRange);
        REQUIRE(leftUnorderedEquals(decodedCapability.DeviceRoles, TestUwbCapability::testUwbCapability.DeviceRoles));
        REQUIRE(leftUnorderedEquals(decodedCapability.RangingConfigurations, TestUwbCapability::testUwbCapability.RangingConfigurations));
        REQUIRE(leftUnorderedEquals(decodedCapability.StsConfigurations, TestUwbCapability::testUwbCapability.StsConfigurations));
        REQUIRE(leftUnorderedEquals(decodedCapability.MultiNodeModes, TestUwbCapability::testUwbCapability.MultiNodeModes));
        REQUIRE(leftUnorderedEquals(decodedCapability.RangingTimeStructs, TestUwbCapability::testUwbCapability.RangingTimeStructs));
        REQUIRE(leftUnorderedEquals(decodedCapability.SchedulingModes, TestUwbCapability::testUwbCapability.SchedulingModes));
        REQUIRE(decodedCapability.HoppingMode == TestUwbCapability::testUwbCapability.HoppingMode);
        REQUIRE(decodedCapability.BlockStriding == TestUwbCapability::testUwbCapability.BlockStriding);
        REQUIRE(decodedCapability.UwbInitiationTime == TestUwbCapability::testUwbCapability.UwbInitiationTime);

        REQUIRE(leftUnorderedEquals(decodedCapability.Channels, TestUwbCapability::testUwbCapability.Channels));
        REQUIRE(leftUnorderedEquals(decodedCapability.RFrameConfigurations, TestUwbCapability::testUwbCapability.RFrameConfigurations));
        REQUIRE(leftUnorderedEquals(decodedCapability.ConvolutionalCodeConstraintLengths, TestUwbCapability::testUwbCapability.ConvolutionalCodeConstraintLengths));
        REQUIRE(leftUnorderedEquals(decodedCapability.BprfParameterSets, TestUwbCapability::testUwbCapability.BprfParameterSets));
        REQUIRE(leftUnorderedEquals(decodedCapability.HprfParameterSets, TestUwbCapability::testUwbCapability.HprfParameterSets));
        REQUIRE(leftUnorderedEquals(decodedCapability.AngleOfArrivalTypes, TestUwbCapability::testUwbCapability.AngleOfArrivalTypes));

        REQUIRE(decodedCapability.AngleOfArrivalFom == TestUwbCapability::testUwbCapability.AngleOfArrivalFom);
        REQUIRE(decodedCapability.ExtendedMacAddress == TestUwbCapability::testUwbCapability.ExtendedMacAddress);
    }
}
