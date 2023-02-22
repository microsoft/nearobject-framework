
#include <random>

#include <catch2/catch_test_macros.hpp>
#include <magic_enum.hpp>

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

    SECTION("UwbSessionState is stable")
    {
        for (const auto& uwbSessionState : magic_enum::enum_values<UwbSessionState>()) {
            test::ValidateRoundtrip(uwbSessionState);
        }
    }

    SECTION("UwbSessionReasonCode is stable")
    {
        for (const auto& uwbReasonCode : magic_enum::enum_values<UwbSessionReasonCode>()) {
            test::ValidateRoundtrip(uwbReasonCode);
        }
    }

    SECTION("UwbRangingMeasurementType is stable")
    {
        for (const auto& uwbMeasurementType : magic_enum::enum_values<UwbRangingMeasurementType>()) {
            test::ValidateRoundtrip(uwbMeasurementType);
        }
    }

    SECTION("UwbApplicationConfigurationParameterType is stable")
    {
        for (const auto& uwbApplicationConfigurationParameterType : magic_enum::enum_values<UwbApplicationConfigurationParameterType>()) {
            test::ValidateRoundtrip(uwbApplicationConfigurationParameterType);
        }
    }

    SECTION("UwbMulticastListStatus is stable")
    {

    }
    
    SECTION("UwbSessionUpdateMulticastListEntry is stable")
    {

    }

    SECTION("UwbSessionUpdateMulicastList is stable")
    {

    }

    SECTION("UwbSessionUpdateMulicastListStatus is stable")
    {

    }

    SECTION("UwbSessionStatus is stable")
    {
        // Generate all possible session states.
        for (const auto& uwbSessionState : magic_enum::enum_values<UwbSessionState>()) {
            // Generate all possible session reason codes.
            for (const auto& uwbSessionReasonCode : magic_enum::enum_values<UwbSessionReasonCode>()) {
                UwbSessionStatus uwbSessionStatus{
                    .SessionId = RandomDistribution(RandomEngine),
                    .State = uwbSessionState,
                    .ReasonCode = uwbSessionReasonCode
                };
                test::ValidateRoundtrip(uwbSessionStatus);
            }
        }
    }
}
