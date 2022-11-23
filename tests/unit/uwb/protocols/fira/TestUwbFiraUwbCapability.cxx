
#include <catch2/catch_test_macros.hpp>

#include <notstd/utility.hxx>

#include <algorithm>
#include <array>
#include <cstdint>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include <TlvBer.hxx>
#include <uwb/protocols/fira/UwbCapability.hxx>

namespace uwb::protocol::fira::TestUwbCapability
{
constexpr auto MultiNodeModesDefault = {
    MultiNodeMode::OneToMany,
};

constexpr auto DeviceRolesDefault = {
    DeviceRole::Initiator,
};

constexpr auto StsConfigurationsDefault = {
    StsConfiguration::Static,
    StsConfiguration::DynamicWithResponderSubSessionKey,
};

constexpr auto RFrameConfigurationsDefault = {
    StsPacketConfiguration::SP1,
    StsPacketConfiguration::SP3,
};

// TODO: spec says "if omitted, AoA is supported". We've assumed "all"
// support, but this could be wrong. Double-check this with FiRa directly.
constexpr auto AngleOfArrivalTypesDefault = {
    AngleOfArrival::Azimuth90,
    AngleOfArrival::Azimuth180,
};

constexpr auto SchedulingModeTypesDefault = {
    SchedulingMode::Time,
};

constexpr auto RangingTimeStructsDefault = {
    RangingMode::Block,
};

constexpr auto RangingConfigurationsDefault = {
    RangingConfiguration{ RangingMethod::OneWay, MeasurementReportMode::None },
    RangingConfiguration{ RangingMethod::SingleSidedTwoWay, MeasurementReportMode::Deferred },
};

constexpr auto ConvolutionalCodeConstraintLengthsDefault = {
    ConvolutionalCodeConstraintLength::K3,
    ConvolutionalCodeConstraintLength::K7,
};

constexpr auto ChannelsDefault = {
    Channel::C5,
    Channel::C6,
    Channel::C8,
    Channel::C9
};

constexpr auto BprfParameterSetsDefault = {
    BprfParameter::Set1,
    BprfParameter::Set2,
    BprfParameter::Set5,
    BprfParameter::Set6,
};

constexpr auto HprfParameterSetsDefault = {
    HprfParameter::Set1,
    HprfParameter::Set2,
    HprfParameter::Set3,
    HprfParameter::Set4,
    HprfParameter::Set5,
    HprfParameter::Set6,
    HprfParameter::Set7,
    HprfParameter::Set35,
};

constexpr uint32_t phyVersionDefault = 0xAABBCCDD;
constexpr uint32_t macVersionDefault = 0xAABBCCEE;

uint32_t FiraPhyVersionRange{ phyVersionDefault };
uint32_t FiraMacVersionRange{ macVersionDefault };
const bool ExtendedMacAddressDefault{ false };
const bool UwbInitiationTimeDefault{ false };
const bool AngleOfArrivalFomDefault{ false };
const bool BlockStridingDefault{ true };
const bool HoppingModeDefault{ true };

// TODO this test code is reliant on the bitmaps staying the same
const std::unordered_map<UwbCapability::ParameterTag, std::vector<uint8_t>> tagAndExpected = {
    { UwbCapability::ParameterTag::FiraPhyVersionRange, { 0xAA, 0xBB, 0xCC, 0xDD } },
    { UwbCapability::ParameterTag::FiraMacVersionRange, { 0xAA, 0xBB, 0xCC, 0xEE } },
    { UwbCapability::ParameterTag::DeviceRoles, { 0x2 } },
    { UwbCapability::ParameterTag::RangingMethod, { 0x3 } },
    { UwbCapability::ParameterTag::StsConfig, { 0x5 } },
    { UwbCapability::ParameterTag::MultiNodeMode, { 0x02 } },
    { UwbCapability::ParameterTag::RangingMode, { 0x1 } },
    { UwbCapability::ParameterTag::ScheduledMode, { 0x2 } },
    { UwbCapability::ParameterTag::HoppingMode, { 1 } },
    { UwbCapability::ParameterTag::BlockStriding, { 1 } },
    { UwbCapability::ParameterTag::UwbInitiationTime, { 0 } },
    { UwbCapability::ParameterTag::Channels, { 0xF } },
    { UwbCapability::ParameterTag::RFrameConfig, { 0xA } },
    { UwbCapability::ParameterTag::CcConstraintLength, { 0x3 } },
    { UwbCapability::ParameterTag::BprfParameterSets, { 0x33 } },
    { UwbCapability::ParameterTag::HprfParameterSets, { 0x4, 0, 0, 0, 0x7F } },
    { UwbCapability::ParameterTag::AoaSupport, { 0x3 } },
    { UwbCapability::ParameterTag::ExtendedMacAddress, { 0 } }
};

const UwbCapability
    testUwbCapability = {
        phyVersionDefault,
        macVersionDefault,
        ExtendedMacAddressDefault,
        UwbInitiationTimeDefault,
        AngleOfArrivalFomDefault,
        BlockStridingDefault,
        HoppingModeDefault,
        MultiNodeModesDefault,
        DeviceRolesDefault,
        StsConfigurationsDefault,
        RFrameConfigurationsDefault,
        AngleOfArrivalTypesDefault,
        SchedulingModeTypesDefault,
        RangingTimeStructsDefault,
        RangingConfigurationsDefault,
        ConvolutionalCodeConstraintLengthsDefault,
        ChannelsDefault,
        BprfParameterSetsDefault,
        HprfParameterSetsDefault
    };
}; // namespace uwb::protocol::fira::TestUwbCapability

TEST_CASE("Encoding into a TlvBer", "[basic]")
{
    using namespace uwb::protocol::fira;

    // Convert a default-constructed UwbCapability object to an OOB data object (tlv).
    const auto tlv = TestUwbCapability::testUwbCapability.ToOobDataObject();

    // Validate tag is as expected.
    const auto tag = tlv->GetTag();
    REQUIRE(tag.size() == 1);
    REQUIRE(tag[0] == UwbCapability::Tag);

    // Validate type is constructed.
    REQUIRE(tlv->IsConstructed());

    // Validate values are as expected.
    const auto values = tlv->GetValues();
    REQUIRE(values.size() == TestUwbCapability::tagAndExpected.size());

    for (auto [tag, expected] : TestUwbCapability::tagAndExpected) {
        // verify that the tag exists in the values
        bool wefoundit = false;
        for (const auto& subtlv : values) {
            wefoundit = (subtlv.GetTag().size() == 1) and (subtlv.GetTag()[0] == notstd::to_underlying(tag));
            if (wefoundit) {
                // validate the value
                const auto tlvValue = subtlv.GetValue();
                REQUIRE(std::equal(std::cbegin(expected), std::cend(expected), std::cbegin(tlvValue), std::cend(tlvValue)));
                break;
            }
        }
        REQUIRE(wefoundit);
    }
}

TEST_CASE("UwbCapability OOB encoding is stable", "[basic][oob][encoding]")
{
    using namespace uwb::protocol::fira;

    SECTION("default value can be round-tripped")
    {
        UwbCapability uwbCapabilityOriginal{};
        const auto uwbCapabilityTlv = uwbCapabilityOriginal.ToOobDataObject();
        const auto uwbCapabilityDecoded = UwbCapability::FromOobDataObject(*uwbCapabilityTlv);
        REQUIRE(uwbCapabilityOriginal == uwbCapabilityDecoded);
    }

    SECTION("complex value can be round-tripped")
    {
        UwbCapability uwbCapabilityOriginal = TestUwbCapability::testUwbCapability;
        const auto uwbCapabilityTlv = uwbCapabilityOriginal.ToOobDataObject();
        const auto uwbCapabilityDecoded = UwbCapability::FromOobDataObject(*uwbCapabilityTlv);
        REQUIRE(uwbCapabilityOriginal == uwbCapabilityDecoded);
    }
}

// TODO find better place for this
template <class T>
bool
leftIsSubset(const std::vector<T>& lhs, const std::vector<T>& rhs)
{
    return std::ranges::all_of(lhs, [&rhs](const auto& elem) {
        return std::ranges::any_of(rhs, [&elem](const auto& elem2) {
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
    using namespace uwb::protocol::fira;

    SECTION("FromOobDataObject correctly throws error with an invalid TlvBer tag")
    {
        const auto tlv = TestUwbCapability::testUwbCapability.ToOobDataObject();

        encoding::TlvBer::Builder builder;
        auto invalidTlv = builder.SetAsCopyOfTlv(*tlv)
                              .SetTag(0xFF)
                              .Build();
        REQUIRE_THROWS(UwbCapability::FromOobDataObject(invalidTlv));
    }
    SECTION("FromOobDataObject works")
    {
        const auto tlv = TestUwbCapability::testUwbCapability.ToOobDataObject();
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

TEST_CASE("UwbCapability can be used in unordered_containers", "[basic][container]")
{
    using namespace uwb::protocol::fira;

    UwbCapability uwbCapabilityDeviceRoleInitiator{};
    uwbCapabilityDeviceRoleInitiator.DeviceRoles = { DeviceRole::Initiator };
    UwbCapability uwbCapabilityDeviceRoleResponder{};
    uwbCapabilityDeviceRoleResponder.DeviceRoles = { DeviceRole::Responder };
    UwbCapability uwbCapabilityDeviceRoleBoth{};
    uwbCapabilityDeviceRoleBoth.DeviceRoles = { DeviceRole::Initiator, DeviceRole::Responder };

    const std::initializer_list<UwbCapability> UwbCapabilities = {
        uwbCapabilityDeviceRoleInitiator,
        uwbCapabilityDeviceRoleResponder,
        uwbCapabilityDeviceRoleBoth
    };

    SECTION("can be used in std::unordered_set")
    {
        std::unordered_set<UwbCapability> uwbCapabilities{};

        const auto insertCapability = [&](const UwbCapability& uwbCapability) -> bool {
            auto [_, inserted] = uwbCapabilities.insert(uwbCapability);
            return inserted;
        };

        for (const auto& uwbCapability : UwbCapabilities) {
            REQUIRE(insertCapability(uwbCapability));
            REQUIRE(!insertCapability(uwbCapability));
        }
    }

    SECTION("can be used as std::unordered_map key")
    {
        unsigned value = 0;
        std::unordered_map<UwbCapability, unsigned> uwbCapabilities{};

        // Populate map with initial objects, ensure all were inserted, implying no existing element.
        for (const auto& uwbCapability : UwbCapabilities) {
            auto [_, inserted] = uwbCapabilities.insert({ uwbCapability, value++ });
            REQUIRE(inserted);
        }

        // Reset value such that insertion sequence matches the one above.
        value = 0;

        // Populate map with same objects, ensure none were inserted, implying elements already exist.
        for (const auto& uwbCapability : UwbCapabilities) {
            auto [_, inserted] = uwbCapabilities.insert({ uwbCapability, value++ });
            REQUIRE(!inserted);
        }
    }
}
