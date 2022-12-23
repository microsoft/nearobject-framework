
#include <array>
#include <cstring>

#include <catch2/catch_test_macros.hpp>

#include <uwb/protocols/fira/FiraDevice.hxx>
#include <windows/uwb/UwbAppConfiguration.hxx>

TEST_CASE("UwbAppConfiguration performs allocation for contained value correctly", "[basic]")
{
    using windows::devices::UwbAppConfigurationParameter;
    using namespace uwb::protocol::fira;

    SECTION("DDI type is correctly reflected")
    {
        constexpr DeviceType deviceTypeExpected = DeviceType::Controller;
        UwbAppConfigurationParameter appConfiguration{ deviceTypeExpected, UWB_APP_CONFIG_PARAM_TYPE_DEVICE_TYPE };

        auto& appConfigurationDdi = appConfiguration.DdiParameter();
        auto* appConfigurationBuffer = appConfiguration.Buffer();
        auto& appConfigurationRaw = *reinterpret_cast<UWB_APP_CONFIG_PARAM*>(appConfigurationBuffer);

        // Validate fields as referenced through the getter and raw buffer all match.
        REQUIRE(appConfigurationDdi.size == appConfigurationRaw.size);
        REQUIRE(appConfigurationDdi.paramType == appConfigurationRaw.paramType);
        REQUIRE(appConfigurationDdi.paramLength == appConfigurationRaw.paramLength);
        REQUIRE(std::memcmp(appConfigurationDdi.paramValue, appConfigurationRaw.paramValue, appConfigurationDdi.paramLength) == 0);

        REQUIRE(appConfigurationDdi == appConfigurationRaw);
    }

    SECTION("explicit size can be specified")
    {
        constexpr std::size_t bufferSize = 1024;
        auto bufferExpected = std::make_unique<uint8_t[]>(bufferSize);

        UwbAppConfigurationParameter appConfiguration{ bufferExpected.get(), UWB_APP_CONFIG_PARAM_TYPE_DST_MAC_ADDRESS, bufferSize };

        auto& appConfigurationDdi = appConfiguration.DdiParameter();
        auto& bufferActual = appConfiguration.Value();
        REQUIRE(bufferActual == bufferExpected.get());
        REQUIRE(std::memcmp(bufferActual, bufferExpected.get(), bufferSize) == 0);
    }

    SECTION("parameter with enum type works")
    {
        constexpr DeviceRole roleExpected{ DeviceRole::Initiator };
        UwbAppConfigurationParameter appConfiguration{ roleExpected, UWB_APP_CONFIG_PARAM_TYPE_DEVICE_ROLE };

        auto& appConfigurationDdi = appConfiguration.DdiParameter();
        auto& roleActual = appConfiguration.Value();
        REQUIRE(roleActual == roleExpected);

        // Below code to validate allocation size requires enabling structure
        // padding. Uncomment it if/when structure padding is enabled in the
        // UwbCx DDI structure definitions.
        {
            // This code assumes that 'paramValue' is the flexible-array member
            // of UWB_APP_CONFIG_PARAM, and that it is defined using the [1]
            // array-subscript method.
            // constexpr std::size_t sizeExpected = sizeof roleExpected + sizeof appConfigurationDdi - sizeof appConfigurationDdi.paramValue;
            // const auto sizeActual = appConfiguration.Size();
            // REQUIRE(sizeActual == sizeExpected);
        }
    }

    SECTION("parameter with POD type (uint8_t) works")
    {
        constexpr uint8_t numberOfControleesExpected = 0xFFU;
        UwbAppConfigurationParameter appConfiguration{ numberOfControleesExpected, UWB_APP_CONFIG_PARAM_TYPE_NUMBER_OF_CONTROLEES };

        auto& appConfigurationDdi = appConfiguration.DdiParameter();
        auto& numberOfControleesActual = appConfiguration.Value();
        REQUIRE(numberOfControleesActual == numberOfControleesExpected);
    }

    SECTION("parameter with POD type (uint16_t) works")
    {
        constexpr uint16_t slotDurationExpected = 0xFFFFU;
        UwbAppConfigurationParameter appConfiguration{ slotDurationExpected, UWB_APP_CONFIG_PARAM_TYPE_SLOT_DURATION };

        auto& appConfigurationDdi = appConfiguration.DdiParameter();
        auto& slotDurationActual = appConfiguration.Value();
        REQUIRE(slotDurationActual == slotDurationExpected);
    }

    SECTION("parameter with POD type (uint32_t) works")
    {
        constexpr uint16_t subSessionIdExpected = 0xFFFFFFFFU;
        UwbAppConfigurationParameter appConfiguration{ subSessionIdExpected, UWB_APP_CONFIG_PARAM_TYPE_SUB_SESSION_ID };

        auto& appConfigurationDdi = appConfiguration.DdiParameter();
        auto& subSessionIdActual = appConfiguration.Value();
        REQUIRE(subSessionIdActual == subSessionIdExpected);
    }

    SECTION("parameter with array type (std::array) works")
    {
        constexpr std::array<uint8_t, 6> staticStsIvExpected{ 0xAAU, 0xBBU, 0xCCU, 0xDDU, 0xEEU, 0xFFU };
        UwbAppConfigurationParameter appConfiguration{ staticStsIvExpected, UWB_APP_CONFIG_PARAM_TYPE_STATIC_STS_IV };

        auto& appConfigurationDdi = appConfiguration.DdiParameter();
        auto& staticStsIvActual = appConfiguration.Value();
        REQUIRE(staticStsIvActual == staticStsIvExpected);
    }

    SECTION("parameter with aggregate array type (std::array<std::array<T,N>,M>) works")
    {
        constexpr std::array<std::array<uint8_t, 2>, 3> dstMacAddressExpected{
            std::array<uint8_t, 2>{ 0xAAU, 0xBBU },
            std::array<uint8_t, 2>{ 0xCCU, 0xDDU },
            std::array<uint8_t, 2>{ 0xEEU, 0xFFU },
        };
        UwbAppConfigurationParameter appConfiguration{ dstMacAddressExpected, UWB_APP_CONFIG_PARAM_TYPE_DST_MAC_ADDRESS };

        auto& appConfigurationDdi = appConfiguration.DdiParameter();
        auto& dstMacAddressActual = appConfiguration.Value();
        REQUIRE(dstMacAddressActual == dstMacAddressExpected);
    }
}
