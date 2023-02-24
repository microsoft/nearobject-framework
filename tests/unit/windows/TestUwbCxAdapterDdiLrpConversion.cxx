
#include <random>

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
    }

    SECTION("UwbRangingData is stable")
    {
    }

    SECTION("UwbNotificationData is stable")
    {
    }
}
