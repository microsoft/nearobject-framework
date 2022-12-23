
#include <catch2/catch_test_macros.hpp>

#include <uwb/protocols/fira/FiraDevice.hxx>
#include <windows/uwb/UwbAppConfiguration.hxx>

TEST_CASE("UwbAppConfiguration performs allocation for contained value correctly", "[basic]")
{
    using windows::devices::UwbAppConfigurationParameter;
    using namespace uwb::protocol::fira;

    SECTION("parameter with POD-type works")
    {
        constexpr DeviceRole roleExpected{ DeviceRole::Initiator };
        UwbAppConfigurationParameter<DeviceRole> appConfiguration{ roleExpected };
        auto& roleActual = appConfiguration.Value();
        REQUIRE(roleActual == roleExpected);

        std::size_t sizeExpected = sizeof roleExpected + 0;
        auto sizeActual = appConfiguration.Size();
        // REQUIRE(sizeActual == sizeExpected);

        auto appConfigurationDdi = appConfiguration.DdiParameter();
    }
}
