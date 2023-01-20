
#include <array>
#include <cstring>

#include <catch2/catch_test_macros.hpp>

#include <uwb/protocols/fira/FiraDevice.hxx>
#include <uwb/protocols/fira/UwbConfiguration.hxx>
#include <windows/devices/uwb/UwbAppConfiguration.hxx>

TEST_CASE("UwbAppConfiguration performs allocation for contained value correctly", "[basic]")
{
    using windows::devices::UwbAppConfigurationParameter;
    using namespace uwb::protocol::fira;

    SECTION("DDI type is correctly reflected")
    {
        constexpr DeviceType deviceTypeExpected = DeviceType::Controller;
        UwbAppConfigurationParameter appConfiguration{ deviceTypeExpected, UWB_APP_CONFIG_PARAM_TYPE_DEVICE_TYPE };

        const auto& appConfigurationDdi = appConfiguration.DdiParameter();
        const auto& appConfigurationBuffer = appConfiguration.DdiBuffer();
        const auto& appConfigurationRaw = *reinterpret_cast<const UWB_APP_CONFIG_PARAM*>(std::data(appConfigurationBuffer));

        // Validate fields as referenced through the getter and raw buffer all match.
        REQUIRE(appConfigurationDdi.size == appConfigurationBuffer.size());
        REQUIRE(appConfigurationDdi.paramType == appConfigurationRaw.paramType);
        REQUIRE(appConfigurationDdi.paramLength == appConfigurationRaw.paramLength);
        REQUIRE(std::memcmp(appConfigurationDdi.paramValue, appConfigurationRaw.paramValue, appConfigurationDdi.paramLength) == 0);

        REQUIRE(appConfigurationDdi == appConfigurationRaw);
    }

    SECTION("DDI type is correctly reflected when using UwbConfiguration parameter tag input")
    {
        constexpr DeviceRole deviceRoleExpected = DeviceRole::Initiator;
        UwbAppConfigurationParameter appConfiguration{ deviceRoleExpected, UwbConfiguration::ParameterTag::DeviceRole };

        const auto& appConfigurationDdi = appConfiguration.DdiParameter();
        const auto& appConfigurationBuffer = appConfiguration.DdiBuffer();
        const auto& appConfigurationRaw = *reinterpret_cast<const UWB_APP_CONFIG_PARAM*>(std::data(appConfigurationBuffer));

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

        const auto& appConfigurationDdi = appConfiguration.DdiParameter();
        const auto& bufferActual = appConfiguration.Value();
        REQUIRE(bufferActual == bufferExpected.get());
        REQUIRE(std::memcmp(bufferActual, bufferExpected.get(), bufferSize) == 0);
    }

    SECTION("parameter with enum type works")
    {
        constexpr DeviceRole roleExpected{ DeviceRole::Initiator };
        UwbAppConfigurationParameter appConfiguration{ roleExpected, UWB_APP_CONFIG_PARAM_TYPE_DEVICE_ROLE };

        const auto& appConfigurationDdi = appConfiguration.DdiParameter();
        const auto& roleActual = appConfiguration.Value();
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

        const auto& appConfigurationDdi = appConfiguration.DdiParameter();
        const auto& numberOfControleesActual = appConfiguration.Value();
        REQUIRE(numberOfControleesActual == numberOfControleesExpected);
    }

    SECTION("parameter with POD type (uint16_t) works")
    {
        constexpr uint16_t slotDurationExpected = 0xFFFFU;
        UwbAppConfigurationParameter appConfiguration{ slotDurationExpected, UWB_APP_CONFIG_PARAM_TYPE_SLOT_DURATION };

        const auto& appConfigurationDdi = appConfiguration.DdiParameter();
        const auto& slotDurationActual = appConfiguration.Value();
        REQUIRE(slotDurationActual == slotDurationExpected);
    }

    SECTION("parameter with POD type (uint32_t) works")
    {
        constexpr uint16_t subSessionIdExpected = 0xFFFFFFFFU;
        UwbAppConfigurationParameter appConfiguration{ subSessionIdExpected, UWB_APP_CONFIG_PARAM_TYPE_SUB_SESSION_ID };

        const auto& appConfigurationDdi = appConfiguration.DdiParameter();
        const auto& subSessionIdActual = appConfiguration.Value();
        REQUIRE(subSessionIdActual == subSessionIdExpected);
    }

    SECTION("parameter with array type (std::array) works")
    {
        constexpr std::array<uint8_t, 6> staticStsIvExpected{ 0xAAU, 0xBBU, 0xCCU, 0xDDU, 0xEEU, 0xFFU };
        UwbAppConfigurationParameter appConfiguration{ staticStsIvExpected, UWB_APP_CONFIG_PARAM_TYPE_STATIC_STS_IV };

        const auto& appConfigurationDdi = appConfiguration.DdiParameter();
        const auto& staticStsIvActual = appConfiguration.Value();
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

        const auto& appConfigurationDdi = appConfiguration.DdiParameter();
        const auto& dstMacAddressActual = appConfiguration.Value();
        REQUIRE(dstMacAddressActual == dstMacAddressExpected);
    }
}

TEST_CASE("UwbSetAppConfigurationParameters performs allocation for contained values correctly", "[basic]")
{
    using windows::devices::UwbAppConfigurationParameter;
    using namespace uwb::protocol::fira;

    SECTION("One parameter case")
    {
        using namespace windows::devices;
        std::vector<std::shared_ptr<IUwbAppConfigurationParameter>> parameters;
        uint32_t expectedSessionId = 1;

        constexpr DeviceRole roleExpected{ DeviceRole::Initiator };
        auto appConfig = std::make_shared<UwbAppConfigurationParameter<DeviceRole>>(roleExpected, UWB_APP_CONFIG_PARAM_TYPE_DEVICE_ROLE);
        parameters.push_back(appConfig);

        UwbSetAppConfigurationParameters testStruct{ parameters, expectedSessionId };

        const auto& ddiParams = testStruct.DdiParameters();

        REQUIRE(ddiParams.sessionId == expectedSessionId);
        REQUIRE(ddiParams.appConfigParamsCount == std::size(parameters));
        REQUIRE(ddiParams.size == appConfig->DdiSize() + offsetof(UWB_SET_APP_CONFIG_PARAMS, appConfigParams[0]));
        REQUIRE(std::memcmp(ddiParams.appConfigParams, appConfig->DdiBuffer().data(), appConfig->DdiSize()) == 0);
    }

    SECTION("One enum parameter and one mac address parameter case")
    {
        using namespace windows::devices;
        std::vector<std::shared_ptr<IUwbAppConfigurationParameter>> parameters;
        uint32_t expectedSessionId = 1;

        // enum
        constexpr DeviceRole roleExpected{ DeviceRole::Initiator };
        auto appConfig1 = std::make_shared<UwbAppConfigurationParameter<DeviceRole>>(roleExpected, UWB_APP_CONFIG_PARAM_TYPE_DEVICE_ROLE);
        parameters.push_back(appConfig1);

        // mac address
        constexpr std::array<std::array<uint8_t, 2>, 3> dstMacAddressExpected{
            std::array<uint8_t, 2>{ 0xAAU, 0xBBU },
            std::array<uint8_t, 2>{ 0xCCU, 0xDDU },
            std::array<uint8_t, 2>{ 0xEEU, 0xFFU },
        };
        auto appConfig2 = std::make_shared<UwbAppConfigurationParameter<std::array<uwb::UwbMacAddress::ShortType, 3Ui64>>>(dstMacAddressExpected, UWB_APP_CONFIG_PARAM_TYPE_DST_MAC_ADDRESS);
        parameters.push_back(appConfig2);

        // construct the set app config params and compare
        UwbSetAppConfigurationParameters testStruct{ parameters, expectedSessionId };

        const auto& ddiParams = testStruct.DdiParameters();
        const uint8_t *appConfigParamsBytes = reinterpret_cast<const uint8_t *>(&ddiParams.appConfigParams[0]);

        REQUIRE(ddiParams.sessionId == expectedSessionId);
        REQUIRE(ddiParams.appConfigParamsCount == std::size(parameters));
        REQUIRE(ddiParams.size == appConfig1->DdiSize() + appConfig2->DdiSize() + offsetof(UWB_SET_APP_CONFIG_PARAMS, appConfigParams[0]));
        REQUIRE(std::memcmp(appConfigParamsBytes, appConfig1->DdiBuffer().data(), appConfig1->DdiSize()) == 0);
        REQUIRE(std::memcmp(&appConfigParamsBytes[appConfig1->DdiSize()], appConfig2->DdiBuffer().data(), appConfig2->DdiSize()) == 0);
    }
}