
#include <concepts>
#include <iterator>
#include <random>
#include <type_traits>

#include <catch2/catch_test_macros.hpp>
#include <magic_enum.hpp>

#include <uwb/UwbMacAddress.hxx>
#include <uwb/protocols/fira/FiraDevice.hxx>
#include <windows/devices/uwb/UwbCxAdapterDdiLrp.hxx>

namespace UwbCxDdi = windows::devices::uwb::ddi::lrp;

namespace windows::devices::uwb::ddi::lrp::test
{
/**
 * @brief Helper to perform a round-trip conversion of a neutral type.
 *
 * This takes a neutral type, converts it to the DDI type, then converts the DDI
 * type back to the neutral type. If the resulting neutral type matches the
 * original neutral type instance, this proves that no information is lost in
 * the conversion, or is "stable".
 *
 * @tparam NeutralT The neutral type.
 * @param instance An instance of the neutral type.
 * @return NeutralT
 */
template <typename NeutralT>
NeutralT
ConvertRoundtrip(const NeutralT& instance)
{
    return UwbCxDdi::To(UwbCxDdi::From(instance));
}

/**
 * @brief Validate a neutral type round-trip conversion works.
 *
 * @tparam NeutralT
 * @param instance An instance of the neutral type to validate.
 */
template <typename NeutralT>
void
ValidateRoundtrip(const NeutralT& instance)
{
    auto instanceCopy = ConvertRoundtrip(instance);
    REQUIRE(instanceCopy == instance);
}

/**
 * @brief Generate a random integral value.
 * 
 * @tparam ReturnT 
 */
template <typename ReturnT = uint32_t>
requires (std::is_same_v<ReturnT, uint8_t> || std::is_same_v<ReturnT, uint16_t> || std::is_same_v<ReturnT, uint32_t>)
ReturnT
GetRandom()
{
    static std::mt19937 engine{ std::random_device{}() };
    static std::uniform_int_distribution<uint32_t> distribution{};

    return static_cast<ReturnT>(distribution(engine));
}

/**
 * @brief Generate a random uwb ranging measurement.
 * 
 * @return ::uwb::protocol::fira::UwbRangingMeasurementData 
 */
::uwb::protocol::fira::UwbRangingMeasurementData
GetRandomUwbMeasurementData()
{
    ::uwb::protocol::fira::UwbRangingMeasurementData uwbRangingMeasurementData {
        .Result = GetRandom<uint16_t>(),
        .FigureOfMerit = GetRandom<uint8_t>(),
    };

    // FoM value of 0 means it is not present, so reset the optional state to match.
    if (uwbRangingMeasurementData.FigureOfMerit == 0) {
        uwbRangingMeasurementData.FigureOfMerit.reset();
    }

    return uwbRangingMeasurementData;
}
} // namespace windows::devices::uwb::ddi::lrp::test

TEST_CASE("ddi <-> neutral type conversions are stable", "[basic][conversion][windows][ddi]")
{
    using namespace windows::devices::uwb::ddi::lrp;
    using namespace uwb::protocol::fira;

    std::mt19937 RandomEngine{ std::random_device{}() };
    std::uniform_int_distribution<uint32_t> RandomDistribution{};

    SECTION("UwbStatus is stable")
    {
        for (const auto& uwbStatusGeneric : magic_enum::enum_values<UwbStatusGeneric>()) {
            const UwbStatus status{ uwbStatusGeneric };
            test::ValidateRoundtrip(status);
        }
    }

    SECTION("UwbDeviceState is stable")
    {
        for (const auto& uwbDeviceState : magic_enum::enum_values<UwbDeviceState>()) {
            // Avoid roundtrip test for neutral enum value which has no corresponding DDI value.
            if (uwbDeviceState != UwbDeviceState::Uninitialized) {
                test::ValidateRoundtrip(uwbDeviceState);
            }
        }
    }

    SECTION("UwbLineOfSightIndicator is stable")
    {
        for (const auto& uwbLineOfSightIndicator : magic_enum::enum_values<UwbLineOfSightIndicator>()) {
            test::ValidateRoundtrip(uwbLineOfSightIndicator);
        }
    }

    SECTION("UwbMulticastAction is stable")
    {
        for (const auto& uwbMulticastAction : magic_enum::enum_values<UwbMulticastAction>()) {
            test::ValidateRoundtrip(uwbMulticastAction);
        }
    }

    SECTION("UwbStatusMulticast is stable")
    {
        for (const auto& uwbStatusMulticast : magic_enum::enum_values<UwbStatusMulticast>()) {
            test::ValidateRoundtrip(uwbStatusMulticast);
        }
    }

    SECTION("UwbMulticastListStatus is stable")
    {
        for (const auto& uwbStatusMulticast : magic_enum::enum_values<UwbStatusMulticast>()) {
            const UwbMulticastListStatus uwbMulticastListStatus{
                .ControleeMacAddress = ::uwb::UwbMacAddress::Random<::uwb::UwbMacAddressType::Short>(),
                .SubSessionId = RandomDistribution(RandomEngine),
                .Status = uwbStatusMulticast
            };
            test::ValidateRoundtrip(uwbMulticastListStatus);
        }
    }

    SECTION("UwbSessionUpdateMulticastListEntry is stable")
    {
        const UwbSessionUpdateMulticastListEntry uwbSessionUpdateMulticastListEntry{
            .ControleeMacAddress = ::uwb::UwbMacAddress::Random<::uwb::UwbMacAddressType::Short>(),
            .SubSessionId = RandomDistribution(RandomEngine)
        };

        test::ValidateRoundtrip(uwbSessionUpdateMulticastListEntry);
    }

    SECTION("UwbSessionUpdateMulicastList is stable")
    {
        const std::vector<UwbSessionUpdateMulticastListEntry> controlees{
            UwbSessionUpdateMulticastListEntry{
                .ControleeMacAddress = ::uwb::UwbMacAddress::Random<::uwb::UwbMacAddressType::Short>(),
                .SubSessionId = RandomDistribution(RandomEngine) },
            UwbSessionUpdateMulticastListEntry{
                .ControleeMacAddress = ::uwb::UwbMacAddress::Random<::uwb::UwbMacAddressType::Short>(),
                .SubSessionId = RandomDistribution(RandomEngine) },
            UwbSessionUpdateMulticastListEntry{
                .ControleeMacAddress = ::uwb::UwbMacAddress::Random<::uwb::UwbMacAddressType::Short>(),
                .SubSessionId = RandomDistribution(RandomEngine) }
        };

        for (const auto& uwbMulticastAction : magic_enum::enum_values<UwbMulticastAction>()) {
            const UwbSessionUpdateMulicastList uwbSessionUpdateMulicastList{
                .SessionId = RandomDistribution(RandomEngine),
                .Action = uwbMulticastAction,
                .Controlees = controlees
            };

            test::ValidateRoundtrip(uwbSessionUpdateMulicastList);
        }
    }

    SECTION("UwbSessionUpdateMulicastListStatus is stable")
    {
        std::vector<UwbMulticastListStatus> uwbMulticastListStatus{};
        for (const auto& uwbStatusMulticast : magic_enum::enum_values<UwbStatusMulticast>()) {
            uwbMulticastListStatus.push_back(UwbMulticastListStatus{
                .ControleeMacAddress = uwb::UwbMacAddress::Random<uwb::UwbMacAddressType::Short>(),
                .SubSessionId = RandomDistribution(RandomEngine),
                .Status = uwbStatusMulticast });
        }

        const UwbSessionUpdateMulicastListStatus uwbSessionUpdateMulicastListStatus{
            .SessionId = RandomDistribution(RandomEngine),
            .Status = std::move(uwbMulticastListStatus)
        };

        test::ValidateRoundtrip(uwbSessionUpdateMulicastListStatus);
    }

    SECTION("UwbRangingMeasurementType is stable")
    {
        for (const auto& uwbMeasurementType : magic_enum::enum_values<UwbRangingMeasurementType>()) {
            test::ValidateRoundtrip(uwbMeasurementType);
        }
    }

    SECTION("UwbSessionReasonCode is stable")
    {
        for (const auto& uwbReasonCode : magic_enum::enum_values<UwbSessionReasonCode>()) {
            test::ValidateRoundtrip(uwbReasonCode);
        }
    }

    SECTION("UwbApplicationConfigurationParameterType is stable")
    {
        for (const auto& uwbApplicationConfigurationParameterType : magic_enum::enum_values<UwbApplicationConfigurationParameterType>()) {
            test::ValidateRoundtrip(uwbApplicationConfigurationParameterType);
        }
    }

    SECTION("UwbSessionState is stable")
    {
        for (const auto& uwbSessionState : magic_enum::enum_values<UwbSessionState>()) {
            test::ValidateRoundtrip(uwbSessionState);
        }
    }

    SECTION("UwbSessionStatus is stable")
    {
        // Generate all possible session states.
        for (const auto& uwbSessionState : magic_enum::enum_values<UwbSessionState>()) {
            // Generate all possible session reason codes.
            for (const auto& uwbSessionReasonCode : magic_enum::enum_values<UwbSessionReasonCode>()) {
                const UwbSessionStatus uwbSessionStatus{
                    .SessionId = RandomDistribution(RandomEngine),
                    .State = uwbSessionState,
                    .ReasonCode = uwbSessionReasonCode
                };
                test::ValidateRoundtrip(uwbSessionStatus);
            }
        }
    }

    SECTION("UwbDeviceInformation is stable")
    {
    }

    SECTION("UwbCapability is stable")
    {
    }

    SECTION("UwbStatusDevice is stable")
    {
        for (const auto& uwbDeviceState : magic_enum::enum_values<UwbDeviceState>()) {
            // Avoid roundtrip test for neutral enum value which has no corresponding DDI value.
            if (uwbDeviceState != UwbDeviceState::Uninitialized) {
                const UwbStatusDevice uwbStatusDevice{
                    .State = uwbDeviceState
                };
                test::ValidateRoundtrip(uwbStatusDevice);
            }
        }
    }

    SECTION("UwbMacAddressType is stable")
    {
        for (const auto& uwbMacAddressType : magic_enum::enum_values<::uwb::UwbMacAddressType>()) {
            test::ValidateRoundtrip(uwbMacAddressType);
        }
    }

    SECTION("UwbMacAddress (short) is stable")
    {
        const auto uwbMacAddress = ::uwb::UwbMacAddress::Random<::uwb::UwbMacAddressType::Short>();
        test::ValidateRoundtrip(uwbMacAddress);
    }

    SECTION("UwbMacAddress (extended) is stable")
    {
        const auto uwbMacAddress = ::uwb::UwbMacAddress::Random<::uwb::UwbMacAddressType::Extended>();
        test::ValidateRoundtrip(uwbMacAddress);
    }

    SECTION("UwbDeviceConfigurationParameterType is stable")
    {
        for (const auto& uwbDeviceConfigurationParameterType : magic_enum::enum_values<UwbDeviceConfigurationParameterType>()) {
            test::ValidateRoundtrip(uwbDeviceConfigurationParameterType);
        }
    }

    SECTION("UwbRangingMeasurement is stable")
    {
        // Build a vector of all possible UwbStatus values.
        std::vector<UwbStatus> uwbStatus{};
        auto uwbStatusGeneric = magic_enum::enum_values<UwbStatusGeneric>();
        auto uwbStatusSession = magic_enum::enum_values<UwbStatusSession>();
        auto uwbStatusRanging = magic_enum::enum_values<UwbStatusRanging>();
        uwbStatus.insert(std::end(uwbStatus), std::make_move_iterator(std::begin(uwbStatusGeneric)), std::make_move_iterator(std::end(uwbStatusGeneric)));
        uwbStatus.insert(std::end(uwbStatus), std::make_move_iterator(std::begin(uwbStatusSession)), std::make_move_iterator(std::end(uwbStatusSession)));
        uwbStatus.insert(std::end(uwbStatus), std::make_move_iterator(std::begin(uwbStatusRanging)), std::make_move_iterator(std::end(uwbStatusRanging)));

        for (const auto& status : uwbStatus) {
            for (const auto& uwbLineOfSightIndicator : magic_enum::enum_values<UwbLineOfSightIndicator>()) {
                for (const auto& uwbMacAddressType : magic_enum::enum_values<::uwb::UwbMacAddressType>()) {
                    const UwbRangingMeasurement uwbRangingMeasurement{
                        .SlotIndex = test::GetRandom<uint8_t>(),
                        .Distance = test::GetRandom<uint16_t>(),
                        .Status = status,
                        .PeerMacAddress = ::uwb::UwbMacAddress::Random(uwbMacAddressType),
                        .LineOfSightIndicator = uwbLineOfSightIndicator,
                        .AoAAzimuth = test::GetRandomUwbMeasurementData(),
                        .AoAElevation = test::GetRandomUwbMeasurementData(),
                        .AoaDestinationAzimuth = test::GetRandomUwbMeasurementData(),
                        .AoaDestinationElevation = test::GetRandomUwbMeasurementData()
                    };

                    // return UwbCxDdi::To(UwbCxDdi::From(instance));
                    auto from = UwbCxDdi::From(uwbRangingMeasurement);
                    auto   to = UwbCxDdi::To(from);
                    INFO("from: " << uwbRangingMeasurement.ToString());
                    INFO("  to: " << to.ToString()); 
                    bool equal = uwbRangingMeasurement == to;
                    CHECK(equal);
                    if (!equal) {
                        INFO("darn");
                    }
                    // test::ValidateRoundtrip(uwbRangingMeasurement);
                }
            }
        }
    }

    SECTION("UwbRangingData is stable")
    {
    }

    SECTION("UwbNotificationData is stable")
    {
    }
}
